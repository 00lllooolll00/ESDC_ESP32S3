#include "impl_tts.h"
#include "bsp_audio.h"
#include "bsp_i2s.h"
#include "bsp_exio.h"
#include "common_header.h"

#include "esp_tts.h"
#include "esp_tts_voice_template.h"
#include "ek_export.h"

#include <stdio.h>
#include <string.h>
#include "esp_heap_caps.h"

EK_LOG_FILE_TAG("impl_tts");

static int _tts_dev_init(void);
static int _tts_dev_deinit(void);
static int _tts_dev_suspend(void);
static int _tts_dev_resume(void);
static int _tts_speak(const char *text);
static int _tts_enable_amp(int enable);

static const plat_dev_ops_t s_tts_base_ops = {
    .init = _tts_dev_init,
    .deinit = _tts_dev_deinit,
    .suspend = _tts_dev_suspend,
    .resume = _tts_dev_resume,
};

static const plat_tts_ops_t s_tts_ops = {
    .speak = _tts_speak,
    .enable_amp = _tts_enable_amp,
};

// 设备实例私有于本文件，通过 impl_tts_dev() 访问
static plat_tts_dev_t s_tts_dev;

plat_tts_dev_t *impl_tts_dev(void)
{
    return &s_tts_dev;
}

// 设备注册：无参，供 EK_EXPORT_COMPONENTS 自动调用
static void impl_tts_register(void)
{
    EK_LOG_INFO("ek_export: COMPONENTS impl_tts_register");
    plat_tts_dev_register(&s_tts_dev, "tts", &s_tts_base_ops, &s_tts_ops, NULL);
}

EK_EXPORT_COMPONENTS(impl_tts_register, 0);

#define TTS_VOICE_PATH  "/vfs/esp_tts_voice_data_xiaole.dat"
#define TTS_SAMPLE_RATE 16000
#define TTS_BIT_WIDTH   16
#define TTS_SPEED       4

static esp_tts_handle_t s_tts_handle = NULL;
static esp_tts_voice_t *s_voice = NULL;
static uint16_t *s_voice_data = NULL;

static int _tts_dev_init(void)
{
    /* 1. 初始化音频链：ES8388 编解码 + I2S 数据通路 */
    bsp_audio_init();
    bsp_i2s_init();
    bsp_i2s_set_samplerate_bits_sample(TTS_SAMPLE_RATE, TTS_BIT_WIDTH);

    /* 初始化后默认关闭功放，播放时再打开 */
    _tts_enable_amp(0);

    /* 2. 从 vfs 加载 voice 数据到 PSRAM */
    FILE *fp = fopen(TTS_VOICE_PATH, "rb");
    if (fp == NULL)
    {
        EK_LOG_ERROR("voice data not found: %s", TTS_VOICE_PATH);
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long voice_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (voice_size <= 0)
    {
        EK_LOG_ERROR("voice data size invalid: %ld", voice_size);
        fclose(fp);
        return -1;
    }
    s_voice_data = (uint16_t *)heap_caps_malloc(voice_size, MALLOC_CAP_SPIRAM);
    if (s_voice_data == NULL)
    {
        EK_LOG_ERROR("alloc voice buffer failed, size=%ld", voice_size);
        fclose(fp);
        return -1;
    }
    size_t rd = fread(s_voice_data, 1, voice_size, fp);
    fclose(fp);
    if (rd != (size_t)voice_size)
    {
        EK_LOG_ERROR("read voice data short: %u/%ld", (unsigned)rd, voice_size);
        return -1;
    }
    EK_LOG_INFO("voice data loaded: %ld bytes", voice_size);

    /* 3. 初始化 voice set + TTS 实例 */
    s_voice = esp_tts_voice_set_init(&esp_tts_voice_template, s_voice_data);
    if (s_voice == NULL)
    {
        EK_LOG_ERROR("esp_tts_voice_set_init failed");
        return -1;
    }
    s_tts_handle = esp_tts_create(s_voice);
    if (s_tts_handle == NULL)
    {
        EK_LOG_ERROR("esp_tts_create failed");
        return -1;
    }
    EK_LOG_INFO("esp_tts ready");
    return 0;
}

static int _tts_speak(const char *text)
{
    if (s_tts_handle == NULL || text == NULL)
    {
        return -1;
    }

    _tts_enable_amp(1);

    /* 确保采样率匹配 TTS（16kHz/16bit）*/
    bsp_i2s_set_samplerate_bits_sample(TTS_SAMPLE_RATE, TTS_BIT_WIDTH);

    if (esp_tts_parse_chinese(s_tts_handle, text))
    {
        int len[1] = { 0 };
        do
        {
            short *pcm = esp_tts_stream_play(s_tts_handle, len, TTS_SPEED);
            if (len[0] > 0)
            {
                /* esp_tts 输出 mono，但 bsp_i2s 配置为 stereo：每个采样需复制到 L/R 两通道，否则声道错位致糊声失真 */
                int n = len[0];
                short *stereo = (short *)malloc((size_t)n * 2 * sizeof(short));
                if (stereo != NULL)
                {
                    for (int i = 0; i < n; i++)
                    {
                        stereo[2 * i] = pcm[i]; /* L */
                        stereo[2 * i + 1] = pcm[i]; /* R */
                    }
                    bsp_i2s_tx_write((uint8_t *)stereo, (uint32_t)(n * 2 * sizeof(short)));
                    free(stereo);
                }
            }
        } while (len[0] > 0);
    }
    vTaskDelay(pdMS_TO_TICKS(50));
    _tts_enable_amp(0);

    return 0;
}

static int _tts_enable_amp(int enable)
{
    bsp_exio_pin_config_t amp_config = {
        .pin = BSP_EXIO_NUM3,
        .mode = BSP_EXIO_PIN_MODE_OUTPUT,
    };
    bsp_exio_conifg_pin(&amp_config);

    // MD8002A SHUTDOWN 当前板级连接为低电平驱动喇叭
    uint8_t level = enable ? 0 : 1;
    bsp_exio_write_pin(BSP_EXIO_NUM3, level);
    EK_LOG_INFO("PA_EN(EXIO3)=%d, amp_enable=%d", level, enable ? 1 : 0);
    return 0;
}

static int _tts_dev_deinit(void)
{
    if (s_tts_handle != NULL)
    {
        esp_tts_destroy(s_tts_handle);
        s_tts_handle = NULL;
    }
    if (s_voice != NULL)
    {
        esp_tts_voice_set_free(s_voice);
        s_voice = NULL;
    }
    if (s_voice_data != NULL)
    {
        free(s_voice_data);
        s_voice_data = NULL;
    }
    return 0;
}

static int _tts_dev_suspend(void)
{
    return 0;
}

static int _tts_dev_resume(void)
{
    return 0;
}
