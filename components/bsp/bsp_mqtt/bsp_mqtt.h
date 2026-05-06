#ifndef BSP_MQTT_H
#define BSP_MQTT_H

#include "common_header.h"

typedef enum
{
    BSP_MQTT_CB_CONNECTED = 1,
    BSP_MQTT_CB_DISCONNECTED,
    BSP_MQTT_CB_SUBSCRIBE,
    BSP_MQTT_CB_UNSUBSCRIBE,
    BSP_MQTT_CB_PUBLISH,

    BSP_MQTT_CB_MAX,
} bsp_mqtt_cb_t;

void bsp_mqtt_init(const char *broker_base_url);
esp_err_t bsp_mqtt_start(void);
esp_err_t bsp_mqtt_stop(void);
esp_err_t bsp_mqtt_reconnect(void);
esp_err_t bsp_mqtt_disconnect(void);
esp_err_t bsp_mqtt_publish(const char *topic, const char *data, int qos);
esp_err_t bsp_mqtt_subscribe(const char *topic, int qos);
esp_err_t bsp_mqtt_unsubscribe(const char *topic);

void bsp_mqtt_register_cb(bsp_mqtt_cb_t type, void (*cb)(void));

#endif // BSP_MQTT_H
