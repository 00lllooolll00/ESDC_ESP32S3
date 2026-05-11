#include "plat_wifi.h"

void plat_wifi_dev_register(
    plat_wifi_dev_t *wifi, const char *name, const plat_dev_ops_t *base_ops, const plat_wifi_ops_t *ops, void *priv)
{
    plat_dev_register(PLAT_GET_BASE(wifi), name, base_ops, priv);
    wifi->ops = ops;
    wifi->state = PLAT_WIFI_DISCONNECTED;
    wifi->event_cb = NULL;
    wifi->event_cb_arg = NULL;
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

int plat_wifi_sta_start(plat_wifi_dev_t *wifi, const char *ssid, const char *passwd)
{
    assert(wifi);
    assert(ssid);

    PLAT_DEV_CHECK(wifi);
    wifi->state = PLAT_WIFI_CONNECTING;

    int err = wifi->ops->sta_start(ssid, passwd);
    if (err) wifi->state = PLAT_WIFI_DISCONNECTED;

    return err;
}

int plat_wifi_sta_stop(plat_wifi_dev_t *wifi)
{
    assert(wifi);

    PLAT_DEV_CHECK(wifi);

    int err = wifi->ops->sta_stop();
    wifi->state = PLAT_WIFI_DISCONNECTED;

    return err;
}

int plat_wifi_scan(plat_wifi_dev_t *wifi, plat_wifi_ap_info_t *ap_info, uint16_t max_count)
{
    assert(wifi);
    assert(ap_info);

    PLAT_DEV_CHECK(wifi);
    return wifi->ops->scan(ap_info, max_count);
}

plat_wifi_state_t plat_wifi_get_state(plat_wifi_dev_t *wifi)
{
    assert(wifi);
    return wifi->state;
}

void plat_wifi_register_event_cb(plat_wifi_dev_t *wifi, plat_wifi_event_cb_t cb, void *arg)
{
    assert(wifi);
    wifi->event_cb = cb;
    wifi->event_cb_arg = arg;
}

void plat_wifi_notify_state(plat_wifi_dev_t *wifi, plat_wifi_state_t state)
{
    assert(wifi);
    wifi->state = state;
    if (wifi->event_cb) wifi->event_cb(state, wifi->event_cb_arg);
}
