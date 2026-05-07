#include "plat_wifi.h"

void plat_wifi_dev_register(
    plat_wifi_dev_t *wifi, const char *name, const plat_dev_ops_t *base_ops, const plat_wifi_ops_t *ops, void *priv)
{
    plat_dev_register(PLAT_GET_BASE(wifi), name, base_ops, priv);
    wifi->ops = ops;
    wifi->cur_state = PLAT_WIFI_DISCONNECTED;
}

int plat_wifi_dev_init(plat_wifi_dev_t *wifi)
{
    assert(wifi);
    return plat_dev_init(PLAT_GET_BASE(wifi));
}

int plat_wifi_dev_deinit(plat_wifi_dev_t *wifi)
{
    assert(wifi);
    return plat_dev_deinit(PLAT_GET_BASE(wifi));
}

int plat_wifi_dev_suspend(plat_wifi_dev_t *wifi)
{
    PLAT_DEV_CHECK(wifi);
    PLAT_DEV_LOCK(wifi);

    int err = PLAT_GET_BASE(wifi)->base_ops->suspend();

    PLAT_DEV_UNLOCK(wifi);
    return err;
}

int plat_wifi_dev_resume(plat_wifi_dev_t *wifi)
{
    PLAT_DEV_CHECK(wifi);
    PLAT_DEV_LOCK(wifi);

    int err = PLAT_GET_BASE(wifi)->base_ops->resume();

    PLAT_DEV_UNLOCK(wifi);
    return err;
}

int plat_wifi_start(plat_wifi_dev_t *wifi, const char *ssid, const char *passswd)
{
    assert(wifi);
    assert(ssid);

    PLAT_DEV_CHECK(wifi);
    PLAT_DEV_LOCK(wifi);

    wifi->cur_state = PLAT_WIFI_CONNECTING;

    int err = wifi->ops->start(ssid, passswd);

    if (!err)
    {
        wifi->cur_state = PLAT_WIFI_CONNECTED;
        memcpy(wifi->cur_info.ssid, ssid, strlen(ssid));
        memcpy(wifi->cur_info.passwd, ssid, strlen(passswd));
    }
    else
    {
        wifi->cur_state = PLAT_WIFI_DISCONNECTED;
    }

    PLAT_DEV_UNLOCK(wifi);
    return err;
}

int plat_wifi_stop(plat_wifi_dev_t *wifi)
{
    assert(wifi);

    PLAT_DEV_CHECK(wifi);
    PLAT_DEV_LOCK(wifi);

    int err = wifi->ops->stop();

    wifi->cur_state = PLAT_WIFI_DISCONNECTED;

    PLAT_DEV_UNLOCK(wifi);
    return err;
}

int plat_wifi_reconnect(plat_wifi_dev_t *wifi, const char *ssid, const char *passswd)
{
    assert(wifi);

    PLAT_DEV_CHECK(wifi);
    PLAT_DEV_LOCK(wifi);

    wifi->cur_state = PLAT_WIFI_CONNECTING;

    int err = wifi->ops->reconnect(ssid, passswd);

    if (!err)
    {
        wifi->cur_state = PLAT_WIFI_CONNECTED;
        memcpy(wifi->cur_info.ssid, ssid, strlen(ssid));
        memcpy(wifi->cur_info.passwd, ssid, strlen(passswd));
    }
    else
    {
        wifi->cur_state = PLAT_WIFI_DISCONNECTED;
    }

    PLAT_DEV_UNLOCK(wifi);
    return err;
}
