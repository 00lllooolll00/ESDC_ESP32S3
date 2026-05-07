#ifndef PLAT_WIFI_H
#define PLAT_WIFI_H

#include "plat_base.h"

typedef struct plat_wifi_info_t plat_wifi_info_t;
typedef struct plat_wifi_ops_t plat_wifi_ops_t;
typedef struct plat_wifi_dev_t plat_wifi_dev_t;

typedef enum
{
    PLAT_WIFI_DISCONNECTED = 0,
    PLAT_WIFI_CONNECTED,
    PLAT_WIFI_CONNECTING,
} plat_wifi_state_t;

struct plat_wifi_info_t
{
    char ssid[32];
    char passwd[64];
};

struct plat_wifi_ops_t
{
    int (*start)(const char *ssid, const char *passwd);
    int (*stop)(void);
    int (*reconnect)(const char *ssid, const char *passwd);
};

struct plat_wifi_dev_t
{
    plat_dev_t base;
    const plat_wifi_ops_t *ops;
    plat_wifi_info_t cur_info;
    plat_wifi_state_t cur_state;
};

int plat_wifi_start(plat_wifi_dev_t *wifi, const char *ssid, const char *passswd);
int plat_wifi_stop(plat_wifi_dev_t *wifi);
int plat_wifi_reconnect(plat_wifi_dev_t *wifi, const char *ssid, const char *passswd);

#endif // PLAT_WIFI_H
