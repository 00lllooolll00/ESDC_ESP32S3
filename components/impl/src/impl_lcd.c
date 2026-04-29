#include "impl_lcd.h"
#include "bsp_lcd.h"

static int _lcd_dev_init(plat_dev_t *dev);
static int _lcd_dev_deinit(plat_dev_t *dev);
static int _lcd_dev_suspend(plat_dev_t *dev);
static int _lcd_dev_resume(plat_dev_t *dev);

static const plat_dev_ops_t s_lcd_base_ops = {
    .init = _lcd_dev_init,
    .deinit = _lcd_dev_deinit,
    .suspend = _lcd_dev_suspend,
    .resume = _lcd_dev_resume,
};

static int _lcd_clear(plat_lcd_dev_t *lcd, uint16_t color);
static int _lcd_flush(plat_lcd_dev_t *lcd, int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer);

static const plat_lcd_ops_t s_lcd_ops = {
    .clear = _lcd_clear,
    .flush = _lcd_flush,
};

int impl_lcd_register(plat_lcd_dev_t *lcd_dev)
{
    plat_lcd_dev_register(lcd_dev, "lcd", &s_lcd_base_ops, &s_lcd_ops, BSP_LCD_WIDTH, BSP_LCD_HEIGHT, NULL);
    return 0;
}

static int _lcd_dev_init(plat_dev_t *dev)
{
    plat_lcd_dev_t *lcd = (plat_lcd_dev_t *)dev;
    bsp_lcd_init((bsp_lcd_trans_done_cb_t)lcd->flush_done_cb, lcd->flush_done_cb_arg);
    return 0;
}

static int _lcd_dev_deinit(plat_dev_t *dev)
{
    return 0;
}

static int _lcd_dev_suspend(plat_dev_t *dev)
{
    return 0;
}

static int _lcd_dev_resume(plat_dev_t *dev)
{
    return 0;
}

static int _lcd_clear(plat_lcd_dev_t *lcd, uint16_t color)
{
    bsp_lcd_clear(color);
    return 0;
}

static int _lcd_flush(plat_lcd_dev_t *lcd, int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer)
{
    bsp_lcd_disp_flush(x0, y0, x1, y1, buffer);
    return 0;
}
