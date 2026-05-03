#include "bsp_rgblcd.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_ldo_regulator.h"
#include "esp_lcd_panel_rgb.h"

typedef struct
{
    bsp_rgblcd_trans_done_cb_t cb;
    void *arg;
} _rgblcd_cb_data_t;

static bool _rgblcd_trans_done_cb(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *arg);
static void _rgblcd_init(void);
static void _rgblcd_exio_pin_config(void);
static void _rgblcd_display_dir(uint8_t dir);

static esp_lcd_panel_handle_t s_rgblcd_handle;
static uint16_t s_width;
static uint16_t s_height;
static DRAM_ATTR void *s_lcd_buffer[2];
static _rgblcd_cb_data_t s_cb_data;

void bsp_rgblcd_init(bsp_rgblcd_trans_done_cb_t cb, void *arg)
{
    if (s_rgblcd_handle) return;

    s_width = BSP_RGBLCD_WIDTH;
    s_height = BSP_RGBLCD_HEIGHT;

    s_cb_data.cb = cb;
    s_cb_data.arg = arg;

    _rgblcd_exio_pin_config();

    BSP_RGBLCD_BL(0);

    _rgblcd_init();

    _rgblcd_display_dir(1); /* 设置横屏 */

    ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(s_rgblcd_handle, 2, &s_lcd_buffer[0], &s_lcd_buffer[1]));

    const esp_lcd_rgb_panel_event_callbacks_t rgb_cbs = {
        .on_color_trans_done = _rgblcd_trans_done_cb,
    };

    ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(s_rgblcd_handle, &rgb_cbs, &s_cb_data));

    BSP_RGBLCD_BL(1);
}

void bsp_rgblcd_clear(uint16_t color)
{
    uint32_t pixel_count = BSP_RGBLCD_WIDTH * BSP_RGBLCD_HEIGHT;
    for (uint8_t fb = 0; fb < 2; fb++)
    {
        uint16_t *buffer = (uint16_t *)s_lcd_buffer[fb];
        if (buffer == NULL)
        {
            continue;
        }

        for (uint32_t i = 0; i < pixel_count; i++)
        {
            buffer[i] = color;
        }
    }
}

void bsp_rgblcd_disp_flush(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer)
{
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(s_rgblcd_handle, x0, y0, x1, y1, buffer));
}

/**
 * @brief       RGBLCD显示方向设置
 * @param       dir:0,竖屏；1,横屏
 * @retval      无
 */
static void _rgblcd_display_dir(uint8_t dir)
{
    if (dir == 0)
    {
        s_height = BSP_RGBLCD_WIDTH;
        s_width = BSP_RGBLCD_HEIGHT;
        ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(s_rgblcd_handle, true)); /* 交换X和Y轴 */
        ESP_ERROR_CHECK(esp_lcd_panel_mirror(s_rgblcd_handle, false, true)); /* 对屏幕的Y轴进行镜像处理 */
    }
    else if (dir == 1)
    {
        s_width = BSP_RGBLCD_WIDTH;
        s_height = BSP_RGBLCD_HEIGHT;
        ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(s_rgblcd_handle, false)); /* 不需要交换X和Y轴 */
        ESP_ERROR_CHECK(esp_lcd_panel_mirror(s_rgblcd_handle, false, false)); /* 对屏幕的XY轴不进行镜像处理 */
    }
}

uint16_t bsp_rgblcd_get_width(void)
{
    return s_width;
}

uint16_t bsp_rgblcd_get_height(void)
{
    return s_height;
}

static bool _rgblcd_trans_done_cb(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *arg)
{
    _rgblcd_cb_data_t *cb_with_arg = (_rgblcd_cb_data_t *)arg;

    if (cb_with_arg && cb_with_arg->cb)
    {
        return cb_with_arg->cb(cb_with_arg->arg);
    }

    return false;
}

static void _rgblcd_init(void)
{
    esp_lcd_rgb_panel_config_t panel_config = {     /* RGBLCD配置结构体 */
        .num_fbs            = 2,                    /* 缓存区数量 */
        .data_width         = 16,                   /* 数据宽度为16位 */
        .psram_trans_align  = 64,                   /* 在PSRAM中分配的缓冲区的对齐 */
        .clk_src            = LCD_CLK_SRC_DEFAULT,  /* RGBLCD外设时钟源 */
        .disp_gpio_num      = GPIO_NUM_NC,          /* 用于显示控制信号,不使用设为-1 */
        .pclk_gpio_num      = BSP_RGBLCD_PCLK_PIN,        /* PCLK信号引脚 */
        .hsync_gpio_num     = GPIO_NUM_NC,          /* HSYNC信号引脚,DE模式可不使用 */
        .vsync_gpio_num     = GPIO_NUM_NC,          /* VSYNC信号引脚,DE模式可不使用 */
        .de_gpio_num        = BSP_RGBLCD_DE_PIN,          /* DE信号引脚 */
        .data_gpio_nums = {                         /* 数据线引脚 */
            BSP_RGBLCD_B3_PIN, BSP_RGBLCD_B4_PIN, BSP_RGBLCD_B5_PIN, BSP_RGBLCD_B6_PIN, BSP_RGBLCD_B7_PIN,
            BSP_RGBLCD_G2_PIN, BSP_RGBLCD_G3_PIN, BSP_RGBLCD_G4_PIN, BSP_RGBLCD_G5_PIN, BSP_RGBLCD_G6_PIN, BSP_RGBLCD_G7_PIN,
            BSP_RGBLCD_R3_PIN, BSP_RGBLCD_R4_PIN, BSP_RGBLCD_R5_PIN, BSP_RGBLCD_R6_PIN, BSP_RGBLCD_R7_PIN,
        },
        .timings = {                                /* RGBLCD时序参数 */
            .pclk_hz            = EK_FREQ_M(20),   /* 像素时钟频率 */
            .h_res              = BSP_RGBLCD_WIDTH,    /* 水平分辨率,即一行中的像素数 */
            .v_res              = BSP_RGBLCD_HEIGHT,   /* 垂直分辨率,即帧中的行数 */
            .hsync_back_porch   = 88,       /* 水平后廊,hsync和行活动数据开始之间的PCLK数 */
            .hsync_front_porch  = 40,       /* 水平前廊,活动数据结束和下一个hsync之间的PCLK数 */
            .hsync_pulse_width  = 3,       /* 垂直同步宽度,单位:行数 */
            .vsync_back_porch   = 32,       /* 垂直后廊,vsync和帧开始之间的无效行数 */
            .vsync_front_porch  = 13,       /* 垂直前廊,帧结束和下一个vsync之间的无效行数 */
            .vsync_pulse_width  = 48,       /* 水平同步宽度,单位:PCLK周期 */
            .flags = {
                .pclk_active_neg = true,            /* RGB数据在下降沿计时 */
            },
        },
        .flags.fb_in_psram = true,                  /* 在PSRAM中分配帧缓冲区 */
        .bounce_buffer_size_px =  800 * 10,  /* 解决写spiflash时,抖动问题 */
    };
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &s_rgblcd_handle)); /* 创建RGB对象 */

    ESP_ERROR_CHECK(esp_lcd_panel_reset(s_rgblcd_handle)); /* 复位RGB屏 */

    ESP_ERROR_CHECK(esp_lcd_panel_init(s_rgblcd_handle)); /* 初始化RGB */
}

static void _rgblcd_exio_pin_config(void)
{
    bsp_exio_pin_config_t exio_lcd_conifg = {
        .mode = BSP_EXIO_PIN_MODE_OUTPUT,
        .pin = BSP_RGBLCD_BL_PIN,
    };
    bsp_exio_conifg_pin(&exio_lcd_conifg);
    exio_lcd_conifg.pin = BSP_RGBLCD_BL_PIN;
    bsp_exio_conifg_pin(&exio_lcd_conifg);
}
