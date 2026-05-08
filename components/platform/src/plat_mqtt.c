#include "plat_mqtt.h"

void plat_mqtt_dev_register(plat_mqtt_dev_t *mqtt,
                            const char *name,
                            const plat_dev_ops_t *base_ops,
                            const plat_mqtt_ops_t *ops,
                            const char *broker_url,
                            void *priv)
{
    plat_dev_register(PLAT_GET_BASE(mqtt), name, base_ops, priv);
    mqtt->ops = ops;
    mqtt->broker_url = broker_url;
}

int plat_mqtt_dev_init(plat_mqtt_dev_t *mqtt)
{
    assert(mqtt);
    return plat_dev_init(PLAT_GET_BASE(mqtt));
}

int plat_mqtt_dev_deinit(plat_mqtt_dev_t *mqtt)
{
    assert(mqtt);
    return plat_dev_deinit(PLAT_GET_BASE(mqtt));
}

int plat_mqtt_dev_suspend(plat_mqtt_dev_t *mqtt)
{
    PLAT_DEV_CHECK(mqtt);
    PLAT_DEV_LOCK(mqtt);

    int err = PLAT_GET_BASE(mqtt)->base_ops->suspend();

    PLAT_DEV_UNLOCK(mqtt);
    return err;
}

int plat_mqtt_dev_resume(plat_mqtt_dev_t *mqtt)
{
    PLAT_DEV_CHECK(mqtt);
    PLAT_DEV_LOCK(mqtt);

    int err = PLAT_GET_BASE(mqtt)->base_ops->resume();

    PLAT_DEV_UNLOCK(mqtt);
    return err;
}

int plat_mqtt_dev_start(plat_mqtt_dev_t *mqtt)
{
    assert(mqtt);
    PLAT_DEV_CHECK(mqtt);
    PLAT_DEV_LOCK(mqtt);
    int err = mqtt->ops->start();
    PLAT_DEV_UNLOCK(mqtt);
    return err;
}

int plat_mqtt_dev_stop(plat_mqtt_dev_t *mqtt)
{
    assert(mqtt);
    PLAT_DEV_CHECK(mqtt);
    PLAT_DEV_LOCK(mqtt);
    int err = mqtt->ops->stop();
    PLAT_DEV_UNLOCK(mqtt);
    return err;
}

int plat_mqtt_dev_reconnect(plat_mqtt_dev_t *mqtt)
{
    assert(mqtt);
    return mqtt->ops->reconnect();
}

int plat_mqtt_dev_disconnect(plat_mqtt_dev_t *mqtt)
{
    assert(mqtt);
    return mqtt->ops->disconnect();
}

int plat_mqtt_dev_publish(plat_mqtt_dev_t *mqtt, const char *topic, const char *data, int len, int qos)
{
    assert(mqtt);
    PLAT_DEV_CHECK(mqtt);
    return mqtt->ops->publish(topic, data, len, qos);
}

int plat_mqtt_dev_subscribe(plat_mqtt_dev_t *mqtt, const char *topic, int qos)
{
    assert(mqtt);
    PLAT_DEV_CHECK(mqtt);
    return mqtt->ops->subscribe(topic, qos);
}

int plat_mqtt_dev_unsubscribe(plat_mqtt_dev_t *mqtt, const char *topic)
{
    assert(mqtt);
    PLAT_DEV_CHECK(mqtt);
    return mqtt->ops->unsubscribe(topic);
}

void plat_mqtt_dev_cb_register(plat_mqtt_dev_t *mqtt, plat_mqtt_cb_t type, void (*cb)(void))
{
    assert(mqtt);
    if (type <= 0 || type >= PLAT_MQTT_CB_MAX) return;
    mqtt->cb[type] = cb;
}

void plat_mqtt_dev_data_cb_register(plat_mqtt_dev_t *mqtt, plat_mqtt_data_cb_t cb)
{
    assert(mqtt);
    mqtt->data_cb = cb;
}