#ifndef IMPL_WIFI_H
#define IMPL_WIFI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_wifi.h"

int impl_wifi_register(plat_wifi_dev_t *wifi_dev);

#ifdef __cplusplus
}
#endif

#endif // IMPL_WIFI_H