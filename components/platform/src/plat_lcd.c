#include "plat_lcd.h"

void plat_lcd_dev_register(plat_lcd_dev_t *lcd,
                           const char *name,
                           const plat_dev_ops_t *base_ops,
                           const plat_lcd_ops_t *ops,
                           uint16_t width,
                           uint16_t height,
                           void *priv)
{
    plat_dev_register(PLAT_GET_BASE(lcd), name, base_ops, priv);
    lcd->ops = ops;
    lcd->width = width;
    lcd->height = height;
    lcd->flush_done_cb = NULL;
    lcd->flush_done_cb_arg = NULL;
}

int plat_lcd_dev_init(plat_lcd_dev_t *lcd)
{
    assert(lcd);
    return plat_dev_init(PLAT_GET_BASE(lcd));
}

int plat_lcd_dev_deinit(plat_lcd_dev_t *lcd)
{
    assert(lcd);
    return plat_dev_deinit(PLAT_GET_BASE(lcd));
}

int plat_lcd_dev_suspend(plat_lcd_dev_t *lcd)
{
    PLAT_DEV_CHECK(lcd);
    PLAT_DEV_LOCK(lcd);

    int err = PLAT_GET_BASE(lcd)->base_ops->suspend();

    PLAT_DEV_UNLOCK(lcd);
    return err;
}

int plat_lcd_dev_resume(plat_lcd_dev_t *lcd)
{
    PLAT_DEV_CHECK(lcd);
    PLAT_DEV_LOCK(lcd);

    int err = PLAT_GET_BASE(lcd)->base_ops->resume();

    PLAT_DEV_UNLOCK(lcd);
    return err;
}

int plat_lcd_dev_flush(plat_lcd_dev_t *lcd, int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer)
{
    // flush 是异步操作，完成由 flush_done_cb 通知，不加 mutex
    if (PLAT_GET_BASE(lcd)->state == PLAT_STATE_SUSPEND) return EBUSY;
    return lcd->ops->flush(x0, y0, x1, y1, buffer);
}

void *plat_lcd_dev_get_fb(plat_lcd_dev_t *lcd, uint8_t index)
{
    if (!lcd->ops->get_fb) return NULL;
    return lcd->ops->get_fb(index);
}

int plat_lcd_dev_clear(plat_lcd_dev_t *lcd, uint16_t color)
{
    PLAT_DEV_CHECK(lcd);
    PLAT_DEV_LOCK(lcd);

    int err = lcd->ops->clear(color);

    PLAT_DEV_UNLOCK(lcd);
    return err;
}
