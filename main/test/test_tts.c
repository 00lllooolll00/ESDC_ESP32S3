#include "common_header.h"
#include "app_tts.h"
#include "ek_export.h"

EK_LOG_FILE_TAG("test_tts");

#define TEST_TTS_DELAY_MS 2000

static void _test_tts_task(void *arg)
{
    (void)arg;

    vTaskDelay(pdMS_TO_TICKS(TEST_TTS_DELAY_MS));
    EK_LOG_INFO("starting fixed TTS test");
    app_tts_say("你好，这是 ESP32-S3 的固定语音测试。");
    vTaskDelete(NULL);
}

static void test_tts_init(void)
{
    BaseType_t ok = xTaskCreate(_test_tts_task, "test tts", 2048, NULL, 2, NULL);
    assert(ok == pdPASS);
}

// EK_EXPORT_APP(test_tts_init, 6);
