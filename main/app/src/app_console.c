#include "app_console.h"
#include "app_tts.h"
#include "app_weather.h"
#include "ek_export.h"
#include "esp_console.h"
#include "esp_log.h"
#include <stdio.h>
#include <string.h>

EK_LOG_FILE_TAG("app_console");

// weather 命令：注入 JSON 天气数据
// 用法：weather {temps:[25.5,26.0,24.8]}（不要加双引号，console 会消费掉）
static int _cmd_weather(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: weather {temps:[25.5,26.0,...]}\n");
        return 1;
    }

    // esp_console_split_argv 消费了双引号，argv[1] 形如 {temps:[25.5,26.0]}
    // cJSON 要求 key 带引号，这里补回 {"temps":[...]}
    char json_buf[512];
    const char *src = argv[1];
    int j = 0;
    for (int i = 0; src[i] && j < (int)sizeof(json_buf) - 2; i++)
    {
        if (src[i] == '{' && src[i + 1] != '"')
        {
            // 在 { 后的 key 前补引号
            json_buf[j++] = '{';
            json_buf[j++] = '"';
            // 复制 key 直到 :
            i++;
            while (src[i] && src[i] != ':' && j < (int)sizeof(json_buf) - 2)
            {
                json_buf[j++] = src[i++];
            }
            // 补 key 后的引号
            json_buf[j++] = '"';
            // 补回当前位置的字符
            if (src[i])
            {
                json_buf[j++] = src[i];
            }
        }
        else
        {
            json_buf[j++] = src[i];
        }
    }
    json_buf[j] = '\0';

    int err = app_weather_inject_json(json_buf);
    if (err != 0)
    {
        printf("inject failed, check json format\n");
        return 1;
    }

    return 0;
}

// weather_demo 命令：注入硬编码演示数据
static int _cmd_weather_demo(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    // 演示数据：20.0°C..28.0°C（×10）
    static const int16_t demo[] = { 200, 210, 230, 250, 270, 280, 270, 250, 230, 210 };
    app_weather_set_forecast(demo, sizeof(demo) / sizeof(demo[0]));
    return 0;
}

// tts 命令：播放输入文本
// 用法：tts 你好，欢迎使用语音播报
static int _cmd_tts(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: tts <text>\n");
        return 1;
    }

    char text[128];
    size_t used = 0;
    text[0] = '\0';
    for (int i = 1; i < argc && used < sizeof(text) - 1; i++)
    {
        int written = snprintf(text + used, sizeof(text) - used, "%s%s", i == 1 ? "" : " ", argv[i]);
        if (written < 0)
        {
            return 1;
        }
        if ((size_t)written >= sizeof(text) - used)
        {
            used = sizeof(text) - 1;
            break;
        }
        used += (size_t)written;
    }

    app_tts_say(text);
    printf("tts queued: %s\n", text);
    return 0;
}

// REPL task：用 fgets 读 stdin，esp_console 分发命令
// 不用 esp_console_new_repl_uart，避免与已安装的 VFS console UART 驱动冲突
static void _console_task(void *arg)
{
    char line[512];

    bool first_prompt = true;

    while (1)
    {
        if (first_prompt)
        {
            fputs("esp32s3> ", stdout);
            fflush(stdout);
            first_prompt = false;
        }

        // VFS UART fgets 可能分段返回，循环读取直到遇到换行符
        line[0] = '\0';
        size_t total_len = 0;
        while (total_len < sizeof(line) - 1)
        {
            if (fgets(line + total_len, sizeof(line) - total_len, stdin) == NULL)
            {
                vTaskDelay(pdMS_TO_TICKS(10));
                continue;
            }
            total_len += strlen(line + total_len);
            if (total_len > 0 && (line[total_len - 1] == '\n' || line[total_len - 1] == '\r'))
            {
                break;
            }
        }

        // 去掉行尾换行符
        while (total_len > 0 && (line[total_len - 1] == '\n' || line[total_len - 1] == '\r'))
        {
            line[--total_len] = '\0';
        }

        // 空行跳过
        if (total_len == 0)
        {
            continue;
        }

        // 分发命令
        int ret = 0;
        esp_err_t err = esp_console_run(line, &ret);
        if (err == ESP_ERR_NOT_FOUND)
        {
            printf("unknown command, type 'help'\n");
        }
        else if (err != ESP_OK)
        {
            printf("error: %s\n", esp_err_to_name(err));
        }

        first_prompt = true;
    }
}

void app_console_init(void)
{
    EK_LOG_INFO("ek_export: APP app_console_init");
#if APP_DEBUG_CONSOLE
    esp_console_config_t console_cfg = ESP_CONSOLE_CONFIG_DEFAULT();
    esp_err_t ret = esp_console_init(&console_cfg);
    if (ret != ESP_OK)
    {
        EK_LOG_ERROR("esp_console_init failed: %s", esp_err_to_name(ret));
        return;
    }

    // 注册 help 命令
    esp_console_register_help_command();

    // 注册 weather 命令
    const esp_console_cmd_t weather_cmd = {
        .command = "weather",
        .help = "Inject weather JSON, e.g. weather {temps:[25.5,26.0]}",
        .hint = NULL,
        .func = _cmd_weather,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&weather_cmd));

    // 注册 weather_demo 命令
    const esp_console_cmd_t demo_cmd = {
        .command = "weather_demo",
        .help = "Inject demo weather data (10 points, 20-28C)",
        .hint = NULL,
        .func = _cmd_weather_demo,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&demo_cmd));

    // 注册 tts 命令
    const esp_console_cmd_t tts_cmd = {
        .command = "tts",
        .help = "Speak text, e.g. tts ni hao",
        .hint = NULL,
        .func = _cmd_tts,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&tts_cmd));

    // 起 console task
    BaseType_t xret = xTaskCreatePinnedToCore(_console_task, "console", 4096, NULL, 2, NULL, 0);
    if (xret != pdTRUE)
    {
        EK_LOG_ERROR("failed to create console task");
        return;
    }

    EK_LOG_INFO("console ready, type 'help' for commands");
#endif // APP_DEBUG_CONSOLE
}

// EK_EXPORT_APP(app_console_init, 6);
