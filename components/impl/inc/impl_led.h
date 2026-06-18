#ifndef IMPL_LED_H
#define IMPL_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_led.h"

// 获取 LED 设备指针（设备实例私有于 impl_led.c）
plat_led_dev_t *impl_led_dev(void);

#ifdef __cplusplus
}
#endif

#endif // IMPL_LED_H
