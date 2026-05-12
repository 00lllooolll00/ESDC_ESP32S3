#ifndef PLAT_LCD_H
#define PLAT_LCD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_base.h"

typedef bool (*plat_lcd_flush_done_cb_t)(void *arg);

typedef struct plat_lcd_dev_t plat_lcd_dev_t;
typedef struct plat_lcd_ops_t plat_lcd_ops_t;

struct plat_lcd_ops_t
{
    int (*clear)(uint16_t color);
    int (*flush)(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer);
    void *(*get_fb)(uint8_t index);
};

struct plat_lcd_dev_t
{
    plat_dev_t base;
    uint16_t width;
    uint16_t height;
    const plat_lcd_ops_t *ops;
    plat_lcd_flush_done_cb_t flush_done_cb;
    void *flush_done_cb_arg;
};

void plat_lcd_dev_register(plat_lcd_dev_t *lcd,
                           const char *name,
                           const plat_dev_ops_t *base_ops,
                           const plat_lcd_ops_t *ops,
                           uint16_t width,
                           uint16_t height,
                           void *priv);
int plat_lcd_dev_init(plat_lcd_dev_t *lcd);
int plat_lcd_dev_deinit(plat_lcd_dev_t *lcd);
int plat_lcd_dev_suspend(plat_lcd_dev_t *lcd);
int plat_lcd_dev_resume(plat_lcd_dev_t *lcd);
int plat_lcd_dev_clear(plat_lcd_dev_t *lcd, uint16_t color);
int plat_lcd_dev_flush(plat_lcd_dev_t *lcd, int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer);
void *plat_lcd_dev_get_fb(plat_lcd_dev_t *lcd, uint8_t index);

#ifdef __cplusplus
}
#endif

#endif // PLAT_LCD_H
