#include "bsp_i2s.h"

static i2s_chan_handle_t s_bsp_tx_handle;
static i2s_chan_handle_t s_bsp_rx_handle;
static i2s_std_config_t s_cfg;

void bsp_i2s_init(void)
{
    i2s_chan_config_t ch_cfg = I2S_CHANNEL_DEFAULT_CONFIG(BSP_I2S, I2S_ROLE_MASTER);
    i2s_new_channel(&ch_cfg, &s_bsp_tx_handle, &s_bsp_rx_handle);
    i2s_std_config_t std_cfg = {
        /* 标准通信模式配置 */
        .clk_cfg  = {
        /* 时钟配置 可用I2S_STD_CLK_DEFAULT_CONFIG(I2S_SAMPLE_RATE)宏函数辅助配置 */
            .sample_rate_hz = BSP_I2S_SAMPLE_RATE,              /* I2S采样率 */
            .clk_src        = I2S_CLK_SRC_DEFAULT,          /* I2S时钟源 */
            .mclk_multiple  = BSP_I2S_MCLK_MULTIPLE,            /* I2S主时钟MCLK相对于采样率的倍数(默认256) */
        },

        .slot_cfg = {
         /* 声道配置,可用I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO)宏函数辅助配置(支持16位宽采样数据) */
            .data_bit_width = I2S_DATA_BIT_WIDTH_16BIT,     /* 声道支持16位宽的采样数据 */
            .slot_bit_width = I2S_SLOT_BIT_WIDTH_AUTO,      /* 通道位宽 */
            .slot_mode      = I2S_SLOT_MODE_STEREO,         /* 立体声 */
            .slot_mask      = I2S_STD_SLOT_BOTH,            /* 启用通道 */
            .ws_width       = I2S_DATA_BIT_WIDTH_16BIT,     /* WS信号位宽 */
            .ws_pol         = false,                        /* WS信号极性 */
            .bit_shift      = true,                         /* 位移位(Philips模式下配置) */
            .left_align     = true,                         /* 左对齐 */
            .big_endian     = false,                        /* 小端模式 */
            .bit_order_lsb  = false                         /* MSB */
        }, 
        
        .gpio_cfg = {
        /* 引脚配置 */
            .mclk = BSP_I2S_MCK_PIN,     /* 主时钟线 */
            .bclk = BSP_I2S_BCK_PIN,     /* 位时钟线 */
            .ws   = BSP_I2S_WS_PIN,      /* 字(声道)选择线 */
            .dout = BSP_I2S_DO_PIN,      /* 串行数据输出线 */
            .din  = BSP_I2S_DI_PIN,      /* 串行数据输入线 */
            .invert_flags = {       /* 引脚翻转(不反相) */
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    };
    s_cfg = std_cfg;

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(s_bsp_tx_handle, &std_cfg)); /* 初始化TX通道 */
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(s_bsp_rx_handle, &std_cfg)); /* 初始化RX通道 */
    ESP_ERROR_CHECK(i2s_channel_enable(s_bsp_tx_handle)); /* 启用TX通道 */
    ESP_ERROR_CHECK(i2s_channel_enable(s_bsp_rx_handle)); /* 启用RX通道 */
}

/**
 * @brief       I2S卸载
 * @param       无
 * @retval      无
 */
void bsp_i2s_deinit(void)
{
    ESP_ERROR_CHECK(i2s_del_channel(s_bsp_tx_handle));
    ESP_ERROR_CHECK(i2s_del_channel(s_bsp_rx_handle));
}

/**
 * @brief       I2S TRX启动
 * @param       无
 * @retval      无
 */
void bsp_i2s_trx_start(void)
{
    ESP_ERROR_CHECK(i2s_channel_enable(s_bsp_tx_handle));
    ESP_ERROR_CHECK(i2s_channel_enable(s_bsp_rx_handle));
}

/**
 * @brief       I2S TRX停止
 * @param       无
 * @retval      无
 */
void bsp_i2s_trx_stop(void)
{
    ESP_ERROR_CHECK(i2s_channel_disable(s_bsp_tx_handle));
    ESP_ERROR_CHECK(i2s_channel_disable(s_bsp_rx_handle));
}

/**
 * @brief       设置采样率和位宽
 * @param       sampleRate  :采样率
 * @param       bits_sample :位宽
 * @note       当前 TTS/ES8388 播放链路使用 16bit 数据，MCLK 默认按 256fs 配置；仅 24bit 场景才需要重新评估 384fs。
 * @retval      无
 */
void bsp_i2s_set_samplerate_bits_sample(int samplerate, int bits_sample)
{
    bsp_i2s_trx_stop();
    /* 如果需要更新声道或时钟配置,需要在更新前先禁用通道 */
    s_cfg.slot_cfg.ws_width = bits_sample; /* 位宽 */
    ESP_ERROR_CHECK(i2s_channel_reconfig_std_slot(s_bsp_tx_handle, &s_cfg.slot_cfg));
    s_cfg.clk_cfg.sample_rate_hz = samplerate; /* 设置采样率 */
    ESP_ERROR_CHECK(i2s_channel_reconfig_std_clock(s_bsp_tx_handle, &s_cfg.clk_cfg));
    bsp_i2s_trx_start();
}

/**
 * @brief       I2S传输数据
 * @param       buffer: 数据存储区的首地址
 * @param       frame_size: 数据大小
 * @retval      发送的数据长度
 */
size_t bsp_i2s_tx_write(uint8_t *buffer, uint32_t frame_size)
{
    size_t bytes_written;
    ESP_ERROR_CHECK(i2s_channel_write(s_bsp_tx_handle, buffer, frame_size, &bytes_written, 1000));
    return bytes_written;
}

/**
 * @brief       I2S读取数据
 * @param       buffer: 读取数据存储区的首地址
 * @param       frame_size: 读取数据大小
 * @retval      接收的数据长度
 */
size_t bsp_i2s_rx_read(uint8_t *buffer, uint32_t frame_size)
{
    size_t bytes_written;
    ESP_ERROR_CHECK(i2s_channel_read(s_bsp_rx_handle, buffer, frame_size, &bytes_written, 1000));
    return bytes_written;
}
