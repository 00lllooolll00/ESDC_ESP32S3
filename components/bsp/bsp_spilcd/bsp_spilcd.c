#include "bsp_spilcd.h"
#include "bsp_spi.h"
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "esp_lcd_panel_st7789.h"

FILE_TAG("bsp_lcd.c");

typedef struct
{
    bsp_spilcd_trans_done_cb_t cb;
    void *arg;
} _spilcd_cb_data_t;

static bool _spilcd_trans_done_cb(esp_lcd_panel_io_handle_t panel, esp_lcd_panel_io_event_data_t *edata, void *arg);
static void _spilcd_exio_pin_init(void);
static esp_lcd_panel_handle_t s_lcd_handle;

void bsp_spilcd_init(bsp_spilcd_trans_done_cb_t cb, void *arg)
{
    static _spilcd_cb_data_t s_cb_data;

    s_cb_data.cb = cb;
    s_cb_data.arg = arg;

    _spilcd_exio_pin_init();
    bsp_spi_init();

    BSP_SPILCD_RST(0);
    vTaskDelay(100);
    BSP_SPILCD_RST(1);
    vTaskDelay(100);

    esp_lcd_panel_io_handle_t io_handle = NULL;

    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = BSP_SPILCD_DC_PIN,
        .cs_gpio_num = BSP_SPILCD_CS_PIN,
        .pclk_hz = EK_FREQ_M(60),
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,

        // 将参数中的真正的回调函数
        // 传入内部的回调中
        .on_color_trans_done = _spilcd_trans_done_cb,
        .user_ctx = &s_cb_data,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(BSP_SPI_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = GPIO_NUM_NC,
        .rgb_ele_order = COLOR_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
        .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &s_lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(s_lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(s_lcd_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_init(s_lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(s_lcd_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(s_lcd_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(s_lcd_handle, true, false));

    bsp_spilcd_clear(0xFFFF);
    BSP_SPILCD_PWR(1);

    LOG_INFO("bsp lcd init ok");
}

void bsp_spilcd_clear(uint16_t color)
{
    uint16_t line = 40;
    uint16_t *buffer = heap_caps_malloc(BSP_SPILCD_WIDTH * sizeof(uint16_t) * line, MALLOC_CAP_DMA);
    uint16_t color_tmp = ((color & 0x00FF) << 8) | ((color & 0xFF00) >> 8);
    if (NULL == buffer)
    {
        LOG_ERROR("Memory for bitmap is not enough");
    }
    else
    {
        for (uint32_t i = 0; i < BSP_SPILCD_WIDTH * line; i++)
        {
            buffer[i] = color_tmp;
        }

        for (uint16_t y = 0; y < BSP_SPILCD_HEIGHT; y += line)
        {
            esp_lcd_panel_draw_bitmap(s_lcd_handle, 0, y, BSP_SPILCD_WIDTH, y + line, buffer);
        }
    }

    heap_caps_free(buffer);
}

void bsp_spilcd_disp_flush(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer)
{
    esp_lcd_panel_draw_bitmap(s_lcd_handle, x0, y0, x1, y1, buffer);
}

/**
 * @brief       LCD 内部默认回调，用于适配 idf 框架
 * @param       panel LCD panel句柄
 * @param       edata 事件指针
 * @param       arg 用户数据
 * @retval      FreeRTOS中是否有更高优先级任务唤醒
 */
static bool _spilcd_trans_done_cb(esp_lcd_panel_io_handle_t panel, esp_lcd_panel_io_event_data_t *edata, void *arg)
{
    // 获取上层真正需要用的回调函数和参数
    _spilcd_cb_data_t *cb_with_arg = (_spilcd_cb_data_t *)arg;

    if (cb_with_arg && cb_with_arg->cb)
    {
        return cb_with_arg->cb(cb_with_arg->arg);
    }

    return false;
}

/**
 * @brief       XL9555 GPIO扩展芯片引脚初始化
 * @param       无
 * @retval      无
 */
static void _spilcd_exio_pin_init(void)
{
    bsp_exio_pin_config_t exio_lcd_conifg = {
        .mode = BSP_EXIO_PIN_MODE_OUTPUT,
        .pin = BSP_SPILCD_PWR_PIN,
    };
    bsp_exio_conifg_pin(&exio_lcd_conifg);
    exio_lcd_conifg.pin = BSP_SPILCD_RST_PIN;
    bsp_exio_conifg_pin(&exio_lcd_conifg);
}
