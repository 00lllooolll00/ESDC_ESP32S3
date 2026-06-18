#include "app_wifi.h"
#include "impl_wifi.h"
#include "ek_export.h"
#include "lvgl.h"

FILE_TAG("app_wifi");

static void _wifi_state_cb(plat_wifi_state_t state, void *arg);
static void _scan_result_cb(int count, plat_wifi_ap_info_t *aps, void *arg);
static void _app_wifi_task(void *arg);

TaskHandle_t g_wifi_task_handle;
static QueueHandle_t s_wifi_cmd_queue;
static app_wifi_evt_cb_t s_evt_cb;
static void *s_evt_cb_arg;

void app_wifi_init(void)
{
    LOG_INFO("ek_export: APP app_wifi_init");
    s_wifi_cmd_queue = xQueueCreate(WIFI_CMD_QUEUE_LEN, sizeof(app_wifi_cmd_msg_t));
    assert(s_wifi_cmd_queue);

    xTaskCreate(_app_wifi_task, "app wifi", 4096, NULL, 3, &g_wifi_task_handle);
}

EK_EXPORT_APP(app_wifi_init, 3);

int app_wifi_send_cmd(const app_wifi_cmd_msg_t *msg, TickType_t timeout)
{
    if (!s_wifi_cmd_queue) return -1;
    return (xQueueSend(s_wifi_cmd_queue, msg, timeout) == pdTRUE) ? 0 : -1;
}

void app_wifi_register_evt_cb(app_wifi_evt_cb_t cb, void *arg)
{
    s_evt_cb = cb;
    s_evt_cb_arg = arg;
}

void app_wifi_unregister_evt_cb(void)
{
    s_evt_cb = NULL;
    s_evt_cb_arg = NULL;
}

static void _wifi_state_cb(plat_wifi_state_t state, void *arg)
{
    app_wifi_evt_t evt;
    switch (state)
    {
        case PLAT_WIFI_CONNECTED:
            evt = APP_WIFI_EVT_CONNECTED;
            break;
        case PLAT_WIFI_DISCONNECTED:
            evt = APP_WIFI_EVT_DISCONNECTED;
            break;
        case PLAT_WIFI_CONNECTING:
            evt = APP_WIFI_EVT_CONNECTING;
            break;
        default:
            return;
    }

    if (s_evt_cb)
    {
        lv_lock();
        s_evt_cb(evt, NULL, s_evt_cb_arg);
        lv_unlock();
    }
}

static void _scan_result_cb(int count, plat_wifi_ap_info_t *aps, void *arg)
{
    app_wifi_scan_result_t *result = (app_wifi_scan_result_t *)arg;

    if (count <= 0)
    {
        free(result);
        if (s_evt_cb)
        {
            lv_lock();
            s_evt_cb(APP_WIFI_EVT_SCAN_FAIL, NULL, s_evt_cb_arg);
            lv_unlock();
        }
    }
    else
    {
        result->count = count;
        if (s_evt_cb)
        {
            lv_lock();
            s_evt_cb(APP_WIFI_EVT_SCAN_DONE, result, s_evt_cb_arg);
            lv_unlock();
        }
    }
}

static void _app_wifi_task(void *arg)
{
    plat_wifi_dev_t *wifi = impl_wifi_dev();
    plat_wifi_dev_init(wifi);
    plat_wifi_register_event_cb(wifi, _wifi_state_cb, NULL);

    app_wifi_cmd_msg_t msg;
    while (1)
    {
        if (xQueueReceive(s_wifi_cmd_queue, &msg, portMAX_DELAY) != pdTRUE)
        {
            continue;
        }

        switch (msg.cmd)
        {
            case APP_WIFI_CMD_SCAN:
                {
                    app_wifi_scan_result_t *result = malloc(sizeof(app_wifi_scan_result_t));
                    if (!result)
                    {
                        if (s_evt_cb)
                        {
                            lv_lock();
                            s_evt_cb(APP_WIFI_EVT_SCAN_FAIL, NULL, s_evt_cb_arg);
                            lv_unlock();
                        }
                        break;
                    }

                    plat_wifi_register_scan_cb(wifi, _scan_result_cb, result);
                    if (plat_wifi_scan(wifi, result->aps, WIFI_SCAN_MAX_AP) != 0)
                    {
                        plat_wifi_register_scan_cb(wifi, NULL, NULL);
                        free(result);
                        if (s_evt_cb)
                        {
                            lv_lock();
                            s_evt_cb(APP_WIFI_EVT_SCAN_FAIL, NULL, s_evt_cb_arg);
                            lv_unlock();
                        }
                    }
                    break;
                }

            case APP_WIFI_CMD_CONNECT:
                plat_wifi_sta_start(wifi, msg.data.connect.ssid, msg.data.connect.password);
                break;

            case APP_WIFI_CMD_DISCONNECT:
                plat_wifi_sta_stop(wifi);
                break;

            default:
                LOG_WARN("unknown wifi cmd: %d", msg.cmd);
                break;
        }
    }
}
