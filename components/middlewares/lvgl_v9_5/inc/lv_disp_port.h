#ifndef LV_DISP_PORT_H
#define LV_DISP_PORT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"
#include "lvgl.h"
#include "plat_lcd.h"

void lv_port_disp_init(plat_lcd_dev_t *lcd_dev);

#ifdef __cplusplus
}
#endif

#endif //  LV_DISP_PORT_H