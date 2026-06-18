#ifndef IMPL_LV_DISP_PORT_H
#define IMPL_LV_DISP_PORT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"
#include "lvgl.h"
#include "plat_lcd.h"

// LVGL 显示端口初始化：设备内部通过 impl_rgblcd_dev() 获取
// 通过 EK_EXPORT_APP 自动注册（在 lv_init 之后执行）
void lv_port_disp_init(void);

#ifdef __cplusplus
}
#endif

#endif // IMPL_LV_DISP_PORT_H
