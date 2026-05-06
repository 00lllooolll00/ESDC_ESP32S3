#include "plat_mqtt.h"

void plat_mqtt_dev_register(plat_mqtt_dev_t *dev,
                            const char *name,
                            const plat_dev_ops_t *base_ops,
                            const plat_mqtt_ops_t *ops,
                            const char *broker_url,
                            void *priv)
{
    plat_dev_register(PLAT_GET_BASE(dev), name, base_ops, priv);
    dev->ops = ops;
    dev->broker_url = broker_url;
    dev->base.state = PLAT_STATE_NONE;
}

int plat_mqtt_dev_init(plat_mqtt_dev_t *dev)
{
    assert(dev);
    return plat_dev_init(PLAT_GET_BASE(dev));
}

int plat_mqtt_dev_deinit(plat_mqtt_dev_t *dev)
{
    assert(dev);
    return plat_dev_deinit(PLAT_GET_BASE(dev));
}

int plat_mqtt_dev_start(plat_mqtt_dev_t *dev)
{
    assert(dev);
    PLAT_DEV_CHECK(dev);
    PLAT_DEV_LOCK(dev);
    int err = dev->ops->start(dev);
    PLAT_DEV_UNLOCK(dev);
    return err;
}

int plat_mqtt_dev_stop(plat_mqtt_dev_t *dev)
{
    assert(dev);
    PLAT_DEV_CHECK(dev);
    PLAT_DEV_LOCK(dev);
    int err = dev->ops->stop(dev);
    PLAT_DEV_UNLOCK(dev);
    return err;
}

int plat_mqtt_dev_reconnect(plat_mqtt_dev_t *dev)
{
    assert(dev);
    PLAT_DEV_CHECK(dev);
    PLAT_DEV_LOCK(dev);
    int err = dev->ops->reconnect(dev);
    PLAT_DEV_UNLOCK(dev);
    return err;
}

int plat_mqtt_dev_disconnect(plat_mqtt_dev_t *dev)
{
    assert(dev);
    PLAT_DEV_CHECK(dev);
    PLAT_DEV_LOCK(dev);
    int err = dev->ops->disconnect(dev);
    PLAT_DEV_UNLOCK(dev);
    return err;
}

int plat_mqtt_dev_publish(plat_mqtt_dev_t *dev, const char *topic, const char *data, int len, int qos)
{
    assert(dev);
    PLAT_DEV_CHECK(dev);
    return dev->ops->publish(dev, topic, data, len, qos);
}

int plat_mqtt_dev_subscribe(plat_mqtt_dev_t *dev, const char *topic, int qos)
{
    assert(dev);
    PLAT_DEV_CHECK(dev);
    return dev->ops->subscribe(dev, topic, qos);
}

int plat_mqtt_dev_unsubscribe(plat_mqtt_dev_t *dev, const char *topic)
{
    assert(dev);
    PLAT_DEV_CHECK(dev);
    return dev->ops->unsubscribe(dev, topic);
}

void plat_mqtt_dev_cb_register(plat_mqtt_dev_t *dev, plat_mqtt_cb_t type, void (*cb)(void))
{
    assert(dev);
    if (type <= 0 || type >= PLAT_MQTT_CB_MAX) return;
    dev->cb[type] = cb;
}

void plat_mqtt_dev_data_cb_register(plat_mqtt_dev_t *dev,
                                    void (*cb)(const char *topic, const char *data, int data_len))
{
    assert(dev);
    dev->data_cb = cb;
}