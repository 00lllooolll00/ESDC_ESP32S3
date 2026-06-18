#ifndef IMPL_LV_INDEV_PORT_H
#define IMPL_LV_INDEV_PORT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"
#include "lvgl.h"
#include "plat_touch.h"

// LVGL 触摸端口初始化：设备内部通过 impl_touch_dev() 获取
// 通过 EK_EXPORT_APP 自动注册（在 lv_init 之后执行）
void lv_port_touch_init(void);

#ifdef __cplusplus
}
#endif

#endif // IMPL_LV_INDEV_PORT_H
