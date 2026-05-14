#include "impl_spilcd.h"
#include "bsp_spilcd.h"

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
static int _lcd_display(bool on);

static const plat_lcd_ops_t s_lcd_ops = {
    .clear = _lcd_clear,
    .flush = _lcd_flush,
    .get_fb = _lcd_get_fb,
    .display = _lcd_display,
};

int impl_spilcd_register(plat_lcd_dev_t *lcd_dev)
{
    s_lcd_dev = lcd_dev;
    plat_lcd_dev_register(lcd_dev, "lcd", &s_lcd_base_ops, &s_lcd_ops, BSP_SPILCD_WIDTH, BSP_SPILCD_HEIGHT, NULL);
    return 0;
}

static int _lcd_dev_init(void)
{
    bsp_spilcd_init((bsp_spilcd_trans_done_cb_t)s_lcd_dev->flush_done_cb, s_lcd_dev->flush_done_cb_arg);
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
    bsp_spilcd_clear(color);
    return 0;
}

static int _lcd_flush(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer)
{
    bsp_spilcd_disp_flush(x0, y0, x1, y1, buffer);
    return 0;
}

static void *_lcd_get_fb(uint8_t index)
{
    (void)index;
    return NULL;
}

static int _lcd_display(bool on)
{
    bsp_spilcd_display(on);
    return 0;
}
