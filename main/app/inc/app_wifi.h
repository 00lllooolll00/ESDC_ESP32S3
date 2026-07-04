#ifndef APP_WIFI_H
#define APP_WIFI_H

#include "common_header.h"
#include "plat_wifi.h"

#define WIFI_SCAN_MAX_AP   10
#define WIFI_CMD_QUEUE_LEN 4

typedef enum
{
    APP_WIFI_CMD_SCAN = 1,
    APP_WIFI_CMD_CONNECT,
    APP_WIFI_CMD_DISCONNECT,
} app_wifi_cmd_t;

typedef enum
{
    APP_WIFI_EVT_SCAN_DONE = 1,
    APP_WIFI_EVT_SCAN_FAIL,
    APP_WIFI_EVT_CONNECTED,
    APP_WIFI_EVT_DISCONNECTED,
    APP_WIFI_EVT_CONNECTING,
} app_wifi_evt_t;

typedef struct
{
    app_wifi_cmd_t cmd;
    union
    {
        struct
        {
            /* scan: no extra params */
        } scan;
        struct
        {
            char ssid[33];
            char password[64];
        } connect;
    } data;
} app_wifi_cmd_msg_t;

typedef struct
{
    int count;
    plat_wifi_ap_info_t aps[WIFI_SCAN_MAX_AP];
} app_wifi_scan_result_t;

typedef void (*app_wifi_evt_cb_t)(app_wifi_evt_t evt, void *data, void *arg);

extern TaskHandle_t g_wifi_task_handle;

void app_wifi_init(void);
int app_wifi_send_cmd(const app_wifi_cmd_msg_t *msg, TickType_t timeout);
void app_wifi_register_evt_cb(app_wifi_evt_cb_t cb, void *arg);
void app_wifi_unregister_evt_cb(void);

/**
 * @brief 查询 WiFi 是否已连接
 *
 * @return true 已连接（PLAT_WIFI_CONNECTED），false 未连接/连接中
 */
bool app_wifi_is_connected(void);

#endif // APP_WIFI_H
