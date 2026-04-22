#include "bsp_lcd.h"

typedef struct
{
    bsp_lcd_trans_done_cb_t cb;
    void *arg;
} _lcd_cb_data_t;

static const char *TAG = "bsp_lcd.c";

static bool _lcd_trans_done_cb(esp_lcd_panel_io_handle_t panel, esp_lcd_panel_io_event_data_t *edata, void *arg);
static void _lcd_spi_init(void);
static void _lcd_exio_pin_init(void);
static esp_lcd_panel_handle_t g_lcd_handle;

void bsp_lcd_init(bsp_lcd_trans_done_cb_t cb, void *arg)
{
    static _lcd_cb_data_t s_cb_data;

    s_cb_data.cb = cb;
    s_cb_data.arg = arg;

    _lcd_exio_pin_init();
    _lcd_spi_init();

    BSP_LCD_RST(0);
    vTaskDelay(100);
    BSP_LCD_RST(1);
    vTaskDelay(100);

    esp_lcd_panel_io_handle_t io_handle = NULL;

    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = BSP_LCD_DC_PIN,
        .cs_gpio_num = BSP_LCD_CS_PIN,
        .pclk_hz = EK_FREQ_M(60),
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,

        // 将参数中的真正的回调函数
        // 传入内部的回调中
        .on_color_trans_done = _lcd_trans_done_cb,
        .user_ctx = &s_cb_data,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(BSP_LCD_SPI_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = GPIO_NUM_NC,
        .rgb_ele_order = COLOR_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
        .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &g_lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(g_lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(g_lcd_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_init(g_lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(g_lcd_handle, true));

    BSP_LCD_PWR(1);

    ESP_LOGI(TAG, "bsp lcd init ok");
}

void bsp_lcd_disp_flush(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer)
{
    esp_lcd_panel_draw_bitmap(g_lcd_handle, x0, y0, x1, y1, buffer);
}

/**
 * @brief       LCD 内部默认回调，用于适配 idf 框架
 * @param       panel LCD panel句柄
 * @param       edata 事件指针
 * @param       arg 用户数据
 * @retval      FreeRTOS中是否有更高优先级任务唤醒
 */
static bool _lcd_trans_done_cb(esp_lcd_panel_io_handle_t panel, esp_lcd_panel_io_event_data_t *edata, void *arg)
{
    // 获取上层真正需要用的回调函数和参数
    _lcd_cb_data_t *cb_with_arg = (_lcd_cb_data_t *)arg;

    if (cb_with_arg && cb_with_arg->cb)
    {
        return cb_with_arg->cb(cb_with_arg->arg);
    }

    return false;
}

/**
 * @brief       SPI总线初始化
 * @param       无
 * @retval      无
 */
static void _lcd_spi_init(void)
{
    spi_bus_config_t buscfg = {
        .sclk_io_num = BSP_LCD_SCLK_PIN,
        .mosi_io_num = BSP_LCD_MOSI_PIN,
        .miso_io_num = BSP_LCD_MISO_PIN,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
        .max_transfer_sz = BSP_LCD_BUFFER_SIZE,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(BSP_LCD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));
}

/**
 * @brief       XL9555 GPIO扩展芯片引脚初始化
 * @param       无
 * @retval      无
 */
static void _lcd_exio_pin_init(void)
{
    bsp_exio_pin_config_t exio_lcd_conifg = {
        .mode = BSP_EXIO_PIN_MODE_OUTPUT,
        .pin = BSP_LCD_PWR_PIN,
    };
    bsp_exio_conifg_pin(&exio_lcd_conifg);
    exio_lcd_conifg.pin = BSP_LCD_RST_PIN;
    bsp_exio_conifg_pin(&exio_lcd_conifg);
}
