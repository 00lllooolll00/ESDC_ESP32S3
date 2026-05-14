#ifndef PLAT_WIFI_H
#define PLAT_WIFI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_base.h"

typedef struct plat_wifi_ap_info_t plat_wifi_ap_info_t;
typedef struct plat_wifi_ops_t plat_wifi_ops_t;
typedef struct plat_wifi_dev_t plat_wifi_dev_t;

typedef enum
{
    PLAT_WIFI_DISCONNECTED = 0,
    PLAT_WIFI_CONNECTING,
    PLAT_WIFI_CONNECTED,
} plat_wifi_state_t;

struct plat_wifi_ap_info_t
{
    char ssid[33];
    uint8_t bssid[6];
    int8_t rssi;
    uint8_t auth_mode;
};

typedef void (*plat_wifi_event_cb_t)(plat_wifi_state_t state, void *arg);
typedef void (*plat_wifi_scan_cb_t)(int count, plat_wifi_ap_info_t *aps, void *arg);

struct plat_wifi_ops_t
{
    int (*sta_start)(const char *ssid, const char *passwd);
    int (*sta_stop)(void);
    int (*scan)(plat_wifi_ap_info_t *ap_info, uint16_t max_count);
};

struct plat_wifi_dev_t
{
    plat_dev_t base;
    const plat_wifi_ops_t *ops;
    volatile plat_wifi_state_t state;
    plat_wifi_event_cb_t event_cb;
    void *event_cb_arg;
    plat_wifi_scan_cb_t scan_cb;
    void *scan_cb_arg;
};

void plat_wifi_dev_register(
    plat_wifi_dev_t *wifi, const char *name, const plat_dev_ops_t *base_ops, const plat_wifi_ops_t *ops, void *priv);
int plat_wifi_dev_init(plat_wifi_dev_t *wifi);
int plat_wifi_dev_deinit(plat_wifi_dev_t *wifi);

int plat_wifi_sta_start(plat_wifi_dev_t *wifi, const char *ssid, const char *passwd);
int plat_wifi_sta_stop(plat_wifi_dev_t *wifi);
int plat_wifi_scan(plat_wifi_dev_t *wifi, plat_wifi_ap_info_t *ap_info, uint16_t max_count);

plat_wifi_state_t plat_wifi_get_state(plat_wifi_dev_t *wifi);
void plat_wifi_register_event_cb(plat_wifi_dev_t *wifi, plat_wifi_event_cb_t cb, void *arg);

void plat_wifi_register_scan_cb(plat_wifi_dev_t *wifi, plat_wifi_scan_cb_t cb, void *arg);
void plat_wifi_notify_scan_result(plat_wifi_dev_t *wifi, int count, plat_wifi_ap_info_t *aps);
void plat_wifi_notify_state(plat_wifi_dev_t *wifi, plat_wifi_state_t state);

#ifdef __cplusplus
}
#endif

#endif // PLAT_WIFI_H
