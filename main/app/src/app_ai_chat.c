#include "common_header.h"

#include "app_ai_chat.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "cJSON.h"
#include <string.h>
#include <stdlib.h>
#include "secrets.h"

#ifndef DEEPSEEK_API_KEY
#error "DEEPSEEK_API_KEY not defined. Copy main/secrets.h.example to main/secrets.h and fill in your key."
#endif
#ifndef DEEPSEEK_API_URL
#error "DEEPSEEK_API_URL not defined. Copy main/secrets.h.example to main/secrets.h and fill in your URL."
#endif

EK_LOG_FILE_TAG("app_ai_chat");

#define DEEPSEEK_MODEL        "deepseek-v4-flash"
#define DEEPSEEK_TIMEOUT_MS   30000
#define DEEPSEEK_BUF_MAX      4096
#define APP_AI_CHAT_QUEUE_LEN  4
#define APP_AI_CHAT_STACK_SIZE 8192
#define APP_AI_CHAT_PRIORITY   3

// JSON 转义后单条消息最大长度
#define APP_AI_CHAT_MAX_ESCAPED 512

typedef struct {
    char text[APP_AI_CHAT_MAX_TEXT];
    app_ai_chat_reply_cb cb;
    void *arg;
} ai_chat_msg_t;

// 对话历史
typedef struct {
    char role[8]; // "user" or "assistant"
    char content[APP_AI_CHAT_MAX_TEXT];
} chat_msg_t;

static chat_msg_t s_history[APP_AI_CHAT_MAX_HISTORY];
static int s_history_count = 0;
static QueueHandle_t s_queue = NULL;
static TaskHandle_t s_task = NULL;

static void _app_ai_chat_task(void *arg);

// JSON 转义：处理 " \ \n \r \t
static void _json_escape(const char *src, char *dst, size_t dst_len)
{
    size_t j = 0;
    for (size_t i = 0; src[i] && j < dst_len - 2; i++)
    {
        switch (src[i])
        {
        case '"':
            if (j < dst_len - 3)
            {
                dst[j++] = '\\';
                dst[j++] = '"';
            }
            break;
        case '\\':
            if (j < dst_len - 3)
            {
                dst[j++] = '\\';
                dst[j++] = '\\';
            }
            break;
        case '\n':
            if (j < dst_len - 3)
            {
                dst[j++] = '\\';
                dst[j++] = 'n';
            }
            break;
        case '\r':
            if (j < dst_len - 3)
            {
                dst[j++] = '\\';
                dst[j++] = 'r';
            }
            break;
        case '\t':
            if (j < dst_len - 3)
            {
                dst[j++] = '\\';
                dst[j++] = 't';
            }
            break;
        default:
            dst[j++] = src[i];
            break;
        }
    }
    dst[j] = '\0';
}

// 添加消息到历史（满后丢弃最旧）
static void _add_history(const char *role, const char *content)
{
    if (s_history_count >= APP_AI_CHAT_MAX_HISTORY)
    {
        memmove(&s_history[0], &s_history[1],
                (APP_AI_CHAT_MAX_HISTORY - 1) * sizeof(chat_msg_t));
        s_history_count = APP_AI_CHAT_MAX_HISTORY - 1;
    }
    strncpy(s_history[s_history_count].role, role,
            sizeof(s_history[0].role) - 1);
    s_history[s_history_count].role[sizeof(s_history[0].role) - 1] = '\0';
    strncpy(s_history[s_history_count].content, content,
            APP_AI_CHAT_MAX_TEXT - 1);
    s_history[s_history_count].content[APP_AI_CHAT_MAX_TEXT - 1] = '\0';
    s_history_count++;
}

void app_ai_chat_init(void)
{
    s_queue = xQueueCreate(APP_AI_CHAT_QUEUE_LEN, sizeof(ai_chat_msg_t));
    assert(s_queue);
}

void app_ai_chat_send(const char *text, app_ai_chat_reply_cb cb, void *arg)
{
    if (s_queue == NULL || text == NULL || cb == NULL)
    {
        return;
    }

    // 延迟创建任务，避免启动期抢占内存（同 app_tts.c 模式）
    if (s_task == NULL)
    {
        BaseType_t ok = xTaskCreate(_app_ai_chat_task, "app_ai_ch",
                                    APP_AI_CHAT_STACK_SIZE, NULL,
                                    APP_AI_CHAT_PRIORITY, &s_task);
        if (ok != pdPASS)
        {
            s_task = NULL;
            EK_LOG_ERROR("create app_ai_chat task failed");
            return;
        }
    }

    ai_chat_msg_t msg = { 0 };
    strncpy(msg.text, text, APP_AI_CHAT_MAX_TEXT - 1);
    msg.cb = cb;
    msg.arg = arg;
    xQueueSend(s_queue, &msg, 0);
}

void app_ai_chat_clear_history(void)
{
    s_history_count = 0;
}

static void _app_ai_chat_task(void *arg)
{
    // 任务启动时一次性分配缓冲区，循环复用
    char *body = malloc(DEEPSEEK_BUF_MAX);
    char *escaped = malloc(APP_AI_CHAT_MAX_ESCAPED);
    char *reply_buf = malloc(APP_AI_CHAT_MAX_REPLY);

    if (!body || !escaped || !reply_buf)
    {
        EK_LOG_ERROR("app_ai_chat: alloc failed");
        free(body);
        free(escaped);
        free(reply_buf);
        s_task = NULL;
        vTaskDelete(NULL);
        return;
    }

    EK_LOG_INFO("app_ai_chat ready");

    while (1)
    {
        ai_chat_msg_t msg;
        if (xQueueReceive(s_queue, &msg, portMAX_DELAY) != pdTRUE)
        {
            continue;
        }

        // 添加用户消息到历史
        _add_history("user", msg.text);

        // 构建 JSON body：system + 对话历史
        int offset = snprintf(body, DEEPSEEK_BUF_MAX,
            "{\"model\":\"%s\",\"messages\":["
            "{\"role\":\"system\",\"content\":\"你是一个智能助手，用简洁的中文回答问题，每次回答不超过100字。\"}",
            DEEPSEEK_MODEL);

        for (int i = 0; i < s_history_count && offset < DEEPSEEK_BUF_MAX - 256; i++)
        {
            _json_escape(s_history[i].content, escaped, APP_AI_CHAT_MAX_ESCAPED);
            offset += snprintf(body + offset, DEEPSEEK_BUF_MAX - offset,
                ",{\"role\":\"%s\",\"content\":\"%s\"}",
                s_history[i].role, escaped);
        }

        offset += snprintf(body + offset, DEEPSEEK_BUF_MAX - offset,
            "],\"stream\":false,\"max_tokens\":200}");

        if (offset >= DEEPSEEK_BUF_MAX)
        {
            EK_LOG_WARN("app_ai_chat: body too long");
            msg.cb("消息过长，请缩短后重试", msg.arg);
            continue;
        }

        // HTTP POST
        esp_http_client_config_t cfg = {
            .url = DEEPSEEK_API_URL,
            .method = HTTP_METHOD_POST,
            .timeout_ms = DEEPSEEK_TIMEOUT_MS,
            .crt_bundle_attach = esp_crt_bundle_attach,
        };
        esp_http_client_handle_t client = esp_http_client_init(&cfg);
        if (!client)
        {
            EK_LOG_WARN("app_ai_chat: client init failed");
            msg.cb("网络初始化失败", msg.arg);
            continue;
        }

        esp_http_client_set_header(client, "Content-Type", "application/json");
        esp_http_client_set_header(client, "Authorization", "Bearer " DEEPSEEK_API_KEY);

        esp_err_t err = esp_http_client_open(client, strlen(body));
        if (err != ESP_OK)
        {
            EK_LOG_WARN("app_ai_chat: open failed: %s", unified_strerror(err));
            esp_http_client_cleanup(client);
            msg.cb("网络连接失败", msg.arg);
            continue;
        }

        int written = esp_http_client_write(client, body, strlen(body));
        if (written < 0)
        {
            EK_LOG_WARN("app_ai_chat: write failed");
            esp_http_client_cleanup(client);
            msg.cb("发送失败", msg.arg);
            continue;
        }

        esp_http_client_fetch_headers(client);
        int status = esp_http_client_get_status_code(client);
        if (status != 200)
        {
            EK_LOG_WARN("app_ai_chat: http status %d", status);
            esp_http_client_cleanup(client);
            msg.cb("服务器错误", msg.arg);
            continue;
        }

        // 读取响应
        int total = 0;
        while (total < DEEPSEEK_BUF_MAX - 1)
        {
            int n = esp_http_client_read(client, body + total,
                                        DEEPSEEK_BUF_MAX - 1 - total);
            if (n <= 0)
            {
                break;
            }
            total += n;
        }
        body[total] = '\0';
        esp_http_client_cleanup(client);

        if (total == 0)
        {
            EK_LOG_WARN("app_ai_chat: empty response");
            msg.cb("空回复", msg.arg);
            continue;
        }

        // 解析 JSON：choices[0].message.content
        reply_buf[0] = '\0';
        bool ok = false;
        cJSON *root = cJSON_Parse(body);
        if (root)
        {
            cJSON *choices = cJSON_GetObjectItem(root, "choices");
            if (choices && cJSON_IsArray(choices))
            {
                cJSON *first = cJSON_GetArrayItem(choices, 0);
                if (first)
                {
                    cJSON *message = cJSON_GetObjectItem(first, "message");
                    if (message)
                    {
                        cJSON *content = cJSON_GetObjectItem(message, "content");
                        if (content && content->valuestring)
                        {
                            strncpy(reply_buf, content->valuestring,
                                    APP_AI_CHAT_MAX_REPLY - 1);
                            reply_buf[APP_AI_CHAT_MAX_REPLY - 1] = '\0';
                            ok = true;
                            EK_LOG_INFO("app_ai_chat: reply = %s", reply_buf);
                        }
                    }
                }
            }
            cJSON_Delete(root);
        }

        if (!ok)
        {
            EK_LOG_WARN("app_ai_chat: json parse failed, body: %.200s", body);
            strncpy(reply_buf, "解析失败", APP_AI_CHAT_MAX_REPLY - 1);
            reply_buf[APP_AI_CHAT_MAX_REPLY - 1] = '\0';
        }

        // 添加 AI 回复到历史
        _add_history("assistant", reply_buf);

        // 回调通知 UI
        msg.cb(reply_buf, msg.arg);
    }
}
