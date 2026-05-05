#ifndef LV_INDEV_PORT_H
#define LV_INDEV_PORT_H

#include "common_header.h"
#include "lvgl.h"
#include "plat_key.h"
#include "plat_touch.h"

void lv_port_key_init(plat_key_dev_t *key_dev);
void lv_port_touch_init(plat_touch_dev_t *dev, uint8_t count);

#endif //  LV_INDEV_PORT_H
