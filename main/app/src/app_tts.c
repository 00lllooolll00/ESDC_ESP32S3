#include "app_tts.h"
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

// 注：app_tts_init 当前未注册到 EK_EXPORT（保持不启动，省 esp-sr 资源）。
// 需要时在此处加 EK_EXPORT_APP(app_tts_init, 3) 即可启用。
void app_tts_init(void)
{
    s_tts_queue = xQueueCreate(APP_TTS_QUEUE_LEN, sizeof(tts_msg_t));
    assert(s_tts_queue);
    xTaskCreate(_app_tts_task, "app tts", APP_TTS_STACK_SIZE, NULL, APP_TTS_PRIORITY, &s_tts_handle);
}

void app_tts_say(const char *text)
{
    if (s_tts_queue == NULL || text == NULL)
    {
        return;
    }
    tts_msg_t msg = { 0 };
    strncpy(msg.text, text, APP_TTS_MAX_TEXT - 1);
    xQueueSend(s_tts_queue, &msg, 0);
}

static void _app_tts_task(void *arg)
{
    /* 初始化 TTS（音频链 + voice 加载 + esp_tts 实例）—— 耗时操作放 task 内不阻塞启动序列 */
    int ret = plat_tts_dev_init(impl_tts_dev());
    if (ret != 0)
    {
        EK_LOG_ERROR("plat_tts_dev_init failed: %d", ret);
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
