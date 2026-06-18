#ifndef IMPL_WIFI_H
#define IMPL_WIFI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_wifi.h"

// 获取 WiFi 设备指针（设备实例私有于 impl_wifi.c）
plat_wifi_dev_t *impl_wifi_dev(void);

#ifdef __cplusplus
}
#endif

#endif // IMPL_WIFI_H
