#ifndef IMPL_RGBLCD_H
#define IMPL_RGBLCD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_lcd.h"

// 获取 RGB LCD 设备指针（设备实例私有于 impl_rgblcd.c）
plat_lcd_dev_t *impl_rgblcd_dev(void);

#ifdef __cplusplus
}
#endif

#endif // IMPL_RGBLCD_H
