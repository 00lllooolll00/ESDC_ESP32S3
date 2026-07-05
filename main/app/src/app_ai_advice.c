#include "common_header.h"

#include "app_ai_advice.h"
#include "app_weather.h"
#include "app_ip_location.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "cJSON.h"
#include <string.h>
#include <stdlib.h>
#include "secrets.h"
#ifndef DEEPSEEK_API_KEY
#    error "DEEPSEEK_API_KEY not defined. Copy main/secrets.h.example to main/secrets.h and fill in your key."
#endif
#ifndef DEEPSEEK_API_URL
#    error "DEEPSEEK_API_URL not defined. Copy main/secrets.h.example to main/secrets.h and fill in your URL."
#endif

EK_LOG_FILE_TAG("app_ai_advice");

#define DEEPSEEK_MODEL      "deepseek-v4-flash"
#define DEEPSEEK_TIMEOUT_MS 30000
#define DEEPSEEK_BUF_MAX    4096

// 用当前天气数据调 DeepSeek 生成 50 字建议
bool app_ai_advice_request(const app_weather_forecast_t *fc, char *out, size_t out_len)
{
    if (!fc || !out || out_len == 0)
    {
        return false;
    }
    out[0] = '\0';

    // 获取城市名
    char city[32] = { 0 };
    app_ip_location_get_city(city, sizeof(city));
    char user_msg[256];
    snprintf(user_msg,
             sizeof(user_msg),
             "今天%s%s，温度%.1f度，湿度%d%%，风速%dkm/h。请给50字以内的生活建议，直接输出建议内容。",
             city,
             app_weather_type_name(fc->type),
             fc->current_temp / 10.0f,
             fc->humidity,
             fc->wind_speed);
    char body[768];
    snprintf(
        body,
        sizeof(body),
        "{\"model\":\"deepseek-v4-flash\","
        "\"messages\":["
        "{\"role\":\"system\",\"content\":\"你是天气助手，根据天气数据给出50字以内的生活建议，直接输出建议内容。\"},"
        "{\"role\":\"user\",\"content\":\"%s\"}"
        "],\"stream\":false,\"max_tokens\":100}",
        user_msg);

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
        EK_LOG_WARN("deepseek: client init failed");
        return false;
    }

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Authorization", "Bearer " DEEPSEEK_API_KEY);

    esp_err_t err = esp_http_client_open(client, strlen(body));
    if (err != ESP_OK)
    {
        EK_LOG_WARN("deepseek: open failed: %s", unified_strerror(err));
        esp_http_client_cleanup(client);
        return false;
    }

    int written = esp_http_client_write(client, body, strlen(body));
    if (written < 0)
    {
        EK_LOG_WARN("deepseek: write failed");
        esp_http_client_cleanup(client);
        return false;
    }

    esp_http_client_fetch_headers(client);
    int status = esp_http_client_get_status_code(client);
    if (status != 200)
    {
        EK_LOG_WARN("deepseek: http status %d", status);
        esp_http_client_cleanup(client);
        return false;
    }

    // 读取响应
    char *buf = malloc(DEEPSEEK_BUF_MAX);
    if (!buf)
    {
        EK_LOG_WARN("deepseek: buf alloc failed");
        esp_http_client_cleanup(client);
        return false;
    }

    int total = 0;
    while (total < DEEPSEEK_BUF_MAX - 1)
    {
        int n = esp_http_client_read(client, buf + total, DEEPSEEK_BUF_MAX - 1 - total);
        if (n <= 0)
        {
            break;
        }
        total += n;
    }
    buf[total] = '\0';
    esp_http_client_cleanup(client);

    if (total == 0)
    {
        EK_LOG_WARN("deepseek: empty response");
        free(buf);
        return false;
    }

    // 解析 JSON：choices[0].message.content
    bool ok = false;
    cJSON *root = cJSON_Parse(buf);
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
                        strncpy(out, content->valuestring, out_len - 1);
                        out[out_len - 1] = '\0';
                        ok = true;
                        EK_LOG_INFO("deepseek: advice = %s", out);
                    }
                }
            }
        }
        cJSON_Delete(root);
    }

    if (!ok)
    {
        EK_LOG_WARN("deepseek: json parse failed, body: %.200s", buf);
    }

    free(buf);
    return ok;
}
