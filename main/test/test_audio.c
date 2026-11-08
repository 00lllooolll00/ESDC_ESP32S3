#include "common_header.h"
#include "bsp_audio.h"
#include "bsp_exio.h"
#include "bsp_i2s.h"
#include "ek_export.h"

EK_LOG_FILE_TAG("test_audio");

#define TEST_AUDIO_DELAY_MS          5000
#define TEST_AUDIO_SAMPLE_RATE        16000
#define TEST_AUDIO_TONE_HZ             1000
#define TEST_AUDIO_AMPLITUDE          20000
#define TEST_AUDIO_FRAMES_PER_BLOCK     128
#define TEST_AUDIO_TASK_STACK           4096
#define TEST_AUDIO_TASK_PRIO               2
static const int16_t s_tone_table[] = {
    TEST_AUDIO_AMPLITUDE, TEST_AUDIO_AMPLITUDE, TEST_AUDIO_AMPLITUDE, TEST_AUDIO_AMPLITUDE,
    TEST_AUDIO_AMPLITUDE, TEST_AUDIO_AMPLITUDE, TEST_AUDIO_AMPLITUDE, TEST_AUDIO_AMPLITUDE,
    -TEST_AUDIO_AMPLITUDE, -TEST_AUDIO_AMPLITUDE, -TEST_AUDIO_AMPLITUDE, -TEST_AUDIO_AMPLITUDE,
    -TEST_AUDIO_AMPLITUDE, -TEST_AUDIO_AMPLITUDE, -TEST_AUDIO_AMPLITUDE, -TEST_AUDIO_AMPLITUDE,
};


static void _test_audio_task(void *arg);
static void _test_audio_set_amp(bool enable);
static void _test_audio_log_es8388(void);

void test_audio_init(void)
{
    BaseType_t ok = xTaskCreate(
        _test_audio_task, "audio_test", TEST_AUDIO_TASK_STACK, NULL, TEST_AUDIO_TASK_PRIO, NULL);
    assert(ok == pdPASS);
}

EK_EXPORT_APP(test_audio_init, 6);


static void _test_audio_task(void *arg)
{
    (void)arg;
    vTaskDelay(pdMS_TO_TICKS(TEST_AUDIO_DELAY_MS));

    // 先关闭功放，再配置 DAC 与 I2S，避免初始化瞬态直接送入扬声器。
    _test_audio_set_amp(false);
    if (bsp_audio_init() != 0)
    {
        EK_LOG_ERROR("audio test: ES8388 init failed");
        vTaskDelete(NULL);
        return;
    }

    bsp_i2s_init();
    bsp_audio_hpvol_set(30);
    _test_audio_log_es8388();

    int16_t pcm[TEST_AUDIO_FRAMES_PER_BLOCK * 2];
    for (size_t i = 0; i < TEST_AUDIO_FRAMES_PER_BLOCK; i++)
    {
        int16_t sample = s_tone_table[i % (sizeof(s_tone_table) / sizeof(s_tone_table[0]))];
        pcm[2 * i] = sample;
        pcm[2 * i + 1] = sample;
    }

    EK_LOG_INFO("audio test: continuous %dHz square wave, amplitude=%d", TEST_AUDIO_TONE_HZ, TEST_AUDIO_AMPLITUDE);
    EK_LOG_INFO("audio test: expected MCLK=4096000Hz, BCLK=512000Hz, LRCK=16000Hz");

    _test_audio_set_amp(true);
    vTaskDelay(pdMS_TO_TICKS(20));
    while (true)
    {
        bsp_i2s_tx_write((uint8_t *)pcm, sizeof(pcm));
    }
}

static void _test_audio_set_amp(bool enable)
{
    bsp_exio_pin_config_t config = {
        .pin = BSP_EXIO_NUM3,
        .mode = BSP_EXIO_PIN_MODE_OUTPUT,
    };
    bsp_exio_conifg_pin(&config);

    // MD8002A 的 SHUTDOWN 为高电平关闭、低电平工作。
    uint8_t level = enable ? 0 : 1;
    bsp_exio_write_pin(BSP_EXIO_NUM3, level);

    uint16_t output = 0;
    if (bsp_exio_read_out(&output) == ESP_OK)
    {
        EK_LOG_INFO("audio test: PA_EN=%d", (output & BSP_EXIO_NUM3) ? 1 : 0);
    }
    else
    {
        EK_LOG_WARN("audio test: PA_EN readback failed");
    }
}

static void _test_audio_log_es8388(void)
{
    static const uint8_t regs[] = { 0x02, 0x04, 0x08, 0x17, 0x18, 0x1A, 0x1B, 0x27, 0x2A, 0x2E, 0x2F, 0x30, 0x31 };

    for (size_t i = 0; i < sizeof(regs) / sizeof(regs[0]); i++)
    {
        uint8_t value = 0;
        if (bsp_audio_read_reg(regs[i], &value) == ESP_OK)
        {
            EK_LOG_INFO("audio test: ES8388[0x%02X]=0x%02X", regs[i], value);
        }
        else
        {
            EK_LOG_ERROR("audio test: ES8388 read failed, reg=0x%02X", regs[i]);
        }
    }
}
