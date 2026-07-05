#include "app_tts.h"
#include "ek_export.h"
#include "plat_tts.h"
#include "impl_tts.h"

#include <string.h>

EK_LOG_FILE_TAG("app_tts.c");

#define APP_TTS_QUEUE_LEN  4
#define APP_TTS_STACK_SIZE 8192
#define APP_TTS_PRIORITY   3
#define APP_TTS_MAX_TEXT   128

typedef struct
{
    char text[APP_TTS_MAX_TEXT];
} tts_msg_t;

static void _app_tts_task(void *arg);

static QueueHandle_t s_tts_queue = NULL;
static TaskHandle_t s_tts_handle = NULL;

// 初始化 TTS 应用队列，音频链和 TTS 任务延迟到首次 app_tts_say 时创建
void app_tts_init(void)
{
    s_tts_queue = xQueueCreate(APP_TTS_QUEUE_LEN, sizeof(tts_msg_t));
    assert(s_tts_queue);
}

EK_EXPORT_APP(app_tts_init, 5);

void app_tts_say(const char *text)
{
    if (s_tts_queue == NULL || text == NULL)
    {
        return;
    }

    if (s_tts_handle == NULL)
    {
        BaseType_t ok =
            xTaskCreate(_app_tts_task, "app tts", APP_TTS_STACK_SIZE, NULL, APP_TTS_PRIORITY, &s_tts_handle);
        if (ok != pdPASS)
        {
            s_tts_handle = NULL;
            EK_LOG_ERROR("create app tts task failed");
            return;
        }
    }

    tts_msg_t msg = { 0 };
    strncpy(msg.text, text, APP_TTS_MAX_TEXT - 1);
    xQueueSend(s_tts_queue, &msg, 0);
}

static void _app_tts_task(void *arg)
{
    // 首次播放时才初始化 TTS（音频链 + voice 加载 + esp_tts 实例），避免启动期抢占 WiFi 内部 RAM
    int ret = plat_tts_dev_init(impl_tts_dev());
    if (ret != 0)
    {
        EK_LOG_ERROR("plat_tts_dev_init failed: %d", ret);
        s_tts_handle = NULL;
        vTaskDelete(NULL);
        return;
    }
    EK_LOG_INFO("app_tts ready");

    tts_msg_t msg;
    while (1)
    {
        if (xQueueReceive(s_tts_queue, &msg, portMAX_DELAY) == pdTRUE)
        {
            plat_tts_dev_speak(impl_tts_dev(), msg.text);
        }
    }
}
