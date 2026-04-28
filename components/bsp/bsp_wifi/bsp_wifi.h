#ifndef BSP_WIFI_H
#define BSP_WIFI_H

#include "platform.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_mac.h"
#include "esp_smartconfig.h"

#define BSP_WIFI_CONNECTED_BIT BIT0
#define BSP_WIFI_FAIL_BIT      BIT1
#define BSP_WIFI_ESPTOUCH_BIT  BIT2
#define BSP_WIFI_MAX_RETRY     5

typedef struct
{
    uint8_t ssid[32];
    uint8_t pswd[64];
    wifi_auth_mode_t auth_mode;
} bsp_wifi_config_t;

EventGroupHandle_t bsp_wifi_sta_init(const bsp_wifi_config_t *sta_cfg);
void bsp_wifi_ap_init(const bsp_wifi_config_t *ap_cfg, uint8_t max_connections);
void bsp_wifi_smartconfig_init(void);

uint16_t bsp_wifi_scan(wifi_ap_record_t *ap_info, uint16_t scan_list_size);

#endif // BSP_WIFI_H
