#ifndef PLAT_MQTT_H
#define PLAT_MQTT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_base.h"

typedef struct plat_mqtt_ops_t plat_mqtt_ops_t;
typedef struct plat_mqtt_dev_t plat_mqtt_dev_t;

typedef enum
{
    PLAT_MQTT_CB_CONNECTED = 1,
    PLAT_MQTT_CB_DISCONNECTED,
    PLAT_MQTT_CB_SUBSCRIBE,
    PLAT_MQTT_CB_UNSUBSCRIBE,
    PLAT_MQTT_CB_PUBLISH,

    PLAT_MQTT_CB_MAX,
} plat_mqtt_cb_t;

typedef void (*plat_mqtt_data_cb_t)(const char *topic, int topic_len, const char *data, int data_len);

struct plat_mqtt_ops_t
{
    int (*start)(void);
    int (*stop)(void);
    int (*reconnect)(void);
    int (*disconnect)(void);
    int (*publish)(const char *topic, const char *data, int len, int qos);
    int (*subscribe)(const char *topic, int qos);
    int (*unsubscribe)(const char *topic);
};

struct plat_mqtt_dev_t
{
    plat_dev_t base;
    const plat_mqtt_ops_t *ops;
    const char *broker_url;
    void *client;
    void (*cb[PLAT_MQTT_CB_MAX])(void);
    plat_mqtt_data_cb_t data_cb;
};

void plat_mqtt_dev_register(plat_mqtt_dev_t *mqtt,
                            const char *name,
                            const plat_dev_ops_t *base_ops,
                            const plat_mqtt_ops_t *ops,
                            const char *broker_url,
                            void *priv);
int plat_mqtt_dev_init(plat_mqtt_dev_t *mqtt);
int plat_mqtt_dev_deinit(plat_mqtt_dev_t *mqtt);
int plat_mqtt_dev_suspend(plat_mqtt_dev_t *mqtt);
int plat_mqtt_dev_resume(plat_mqtt_dev_t *mqtt);

int plat_mqtt_dev_start(plat_mqtt_dev_t *mqtt);
int plat_mqtt_dev_stop(plat_mqtt_dev_t *mqtt);
int plat_mqtt_dev_reconnect(plat_mqtt_dev_t *mqtt);
int plat_mqtt_dev_disconnect(plat_mqtt_dev_t *mqtt);

int plat_mqtt_dev_publish(plat_mqtt_dev_t *mqtt, const char *topic, const char *data, int len, int qos);
int plat_mqtt_dev_subscribe(plat_mqtt_dev_t *mqtt, const char *topic, int qos);
int plat_mqtt_dev_unsubscribe(plat_mqtt_dev_t *mqtt, const char *topic);

void plat_mqtt_dev_cb_register(plat_mqtt_dev_t *mqtt, plat_mqtt_cb_t type, void (*cb)(void));
void plat_mqtt_dev_data_cb_register(plat_mqtt_dev_t *mqtt, plat_mqtt_data_cb_t cb);

#ifdef __cplusplus
}
#endif

#endif // PLAT_MQTT_H
