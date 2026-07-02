#include "impl_rgblcd.h"
#include "bsp_rgblcd.h"
#include "ek_export.h"

EK_LOG_FILE_TAG("impl_rgblcd.c");

static int _lcd_dev_init(void);
static int _lcd_dev_deinit(void);
static int _lcd_dev_suspend(void);
static int _lcd_dev_resume(void);

static const plat_dev_ops_t s_lcd_base_ops = {
    .init = _lcd_dev_init,
    .deinit = _lcd_dev_deinit,
    .suspend = _lcd_dev_suspend,
    .resume = _lcd_dev_resume,
};

static int _lcd_clear(uint16_t color);
static int _lcd_flush(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer);
static void *_lcd_get_fb(uint8_t index);
static int _lcd_display(bool on);

static const plat_lcd_ops_t s_lcd_ops = {
    .clear = _lcd_clear,
    .flush = _lcd_flush,
    .get_fb = _lcd_get_fb,
    .display = _lcd_display,
};

// 设备实例私有于本文件，通过 impl_rgblcd_dev() 访问
static plat_lcd_dev_t s_lcd_dev;

plat_lcd_dev_t *impl_rgblcd_dev(void)
{
    return &s_lcd_dev;
}

// 设备注册：无参，供 EK_EXPORT_COMPONENTS 自动调用
static void impl_rgblcd_register(void)
{
    EK_LOG_INFO("ek_export: COMPONENTS impl_rgblcd_register");
    plat_lcd_dev_register(&s_lcd_dev, "lcd", &s_lcd_base_ops, &s_lcd_ops, BSP_RGBLCD_WIDTH, BSP_RGBLCD_HEIGHT, NULL);
}

EK_EXPORT_COMPONENTS(impl_rgblcd_register, 0);

static int _lcd_dev_init(void)
{
    bsp_rgblcd_init((bsp_rgblcd_trans_done_cb_t)s_lcd_dev.flush_done_cb, s_lcd_dev.flush_done_cb_arg);
    return 0;
}

static int _lcd_dev_deinit(void)
{
    return 0;
}

static int _lcd_dev_suspend(void)
{
    return 0;
}

static int _lcd_dev_resume(void)
{
    return 0;
}

static int _lcd_clear(uint16_t color)
{
    bsp_rgblcd_clear(color);
    return 0;
}

static int _lcd_flush(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer)
{
    bsp_rgblcd_disp_flush(x0, y0, x1, y1, buffer);
    return 0;
}

static void *_lcd_get_fb(uint8_t index)
{
    return bsp_rgb_get_fb(index);
}

static int _lcd_display(bool on)
{
    bsp_rgblcd_display(on);
    return 0;
}
