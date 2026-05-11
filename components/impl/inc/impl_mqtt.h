#ifndef IMPL_MQTT_H
#define IMPL_MQTT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_mqtt.h"

int impl_mqtt_register(plat_mqtt_dev_t *mqtt_dev);

#ifdef __cplusplus
}
#endif

#endif // IMPL_MQTT_H