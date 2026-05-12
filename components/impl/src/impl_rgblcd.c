#include "impl_rgblcd.h"
#include "bsp_rgblcd.h"

static plat_lcd_dev_t *s_lcd_dev;

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

static const plat_lcd_ops_t s_lcd_ops = {
    .clear = _lcd_clear,
    .flush = _lcd_flush,
    .get_fb = _lcd_get_fb,
};

int impl_rgblcd_register(plat_lcd_dev_t *lcd_dev)
{
    s_lcd_dev = lcd_dev;
    plat_lcd_dev_register(lcd_dev, "lcd", &s_lcd_base_ops, &s_lcd_ops, BSP_RGBLCD_WIDTH, BSP_RGBLCD_HEIGHT, NULL);
    return 0;
}

static int _lcd_dev_init(void)
{
    bsp_rgblcd_init((bsp_rgblcd_trans_done_cb_t)s_lcd_dev->flush_done_cb, s_lcd_dev->flush_done_cb_arg);
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
