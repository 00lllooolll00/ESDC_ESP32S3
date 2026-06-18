#ifndef IMPL_MQTT_H
#define IMPL_MQTT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_mqtt.h"

// 获取 MQTT 设备指针（设备实例私有于 impl_mqtt.c）
plat_mqtt_dev_t *impl_mqtt_dev(void);

#ifdef __cplusplus
}
#endif

#endif // IMPL_MQTT_H
