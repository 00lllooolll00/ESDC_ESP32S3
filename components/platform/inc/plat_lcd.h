#ifndef PLAT_LCD_H
#define PLAT_LCD_H

#include "plat_base.h"

typedef struct
{
    plat_dev_t base;
    uint16_t width;
    uint16_t height;
    void *ops;
} plat_lcd_dev_t;

#endif // PLAT_LCD_H
