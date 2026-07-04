#include "app_wifi.h"
#include "impl_wifi.h"
#include "app_wifi_store.h"
#include "ek_export.h"
#include "lvgl.h"

EK_LOG_FILE_TAG("app_wifi");

static void _wifi_state_cb(plat_wifi_state_t state, void *arg);
static void _scan_result_cb(int count, plat_wifi_ap_info_t *aps, void *arg);
static void _app_wifi_task(void *arg);

// 自动扫描用：阻塞等待异步扫描结果
static SemaphoreHandle_t s_auto_scan_sem;
static int s_auto_scan_count;
static plat_wifi_ap_info_t *s_auto_scan_aps;

TaskHandle_t g_wifi_task_handle;
static QueueHandle_t s_wifi_cmd_queue;
#define APP_WIFI_EVT_CB_MAX 4
static app_wifi_evt_cb_t s_evt_cbs[APP_WIFI_EVT_CB_MAX];
static void *s_evt_cb_args[APP_WIFI_EVT_CB_MAX];

void app_wifi_init(void)
{
    EK_LOG_INFO("ek_export: APP app_wifi_init");
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
    for (int i = 0; i < APP_WIFI_EVT_CB_MAX; i++)
    {
        if (!s_evt_cbs[i])
        {
            s_evt_cbs[i] = cb;
            s_evt_cb_args[i] = arg;
            return;
        }
    }
    EK_LOG_WARN("wifi evt cb slots full, cannot register");
}

void app_wifi_unregister_evt_cb(void)
{
    for (int i = 0; i < APP_WIFI_EVT_CB_MAX; i++)
    {
        s_evt_cbs[i] = NULL;
        s_evt_cb_args[i] = NULL;
    }
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

    for (int i = 0; i < APP_WIFI_EVT_CB_MAX; i++)
    {
        if (s_evt_cbs[i])
        {
            lv_lock();
            s_evt_cbs[i](evt, NULL, s_evt_cb_args[i]);
            lv_unlock();
        }
    }
}

static void _scan_result_cb(int count, plat_wifi_ap_info_t *aps, void *arg)
{
    app_wifi_scan_result_t *result = (app_wifi_scan_result_t *)arg;

    if (count <= 0)
    {
        free(result);
        for (int i = 0; i < APP_WIFI_EVT_CB_MAX; i++)
        {
            if (s_evt_cbs[i])
            {
                lv_lock();
                s_evt_cbs[i](APP_WIFI_EVT_SCAN_FAIL, NULL, s_evt_cb_args[i]);
                lv_unlock();
            }
        }
    }
    else
    {
        result->count = count;
        for (int i = 0; i < APP_WIFI_EVT_CB_MAX; i++)
        {
            if (s_evt_cbs[i])
            {
                lv_lock();
                s_evt_cbs[i](APP_WIFI_EVT_SCAN_DONE, result, s_evt_cb_args[i]);
                lv_unlock();
            }
        }
        free(result);
    }
}

// 自动扫描结果回调：存结果并释放信号量唤醒 task
static void _auto_scan_cb(int count, plat_wifi_ap_info_t *aps, void *arg)
{
    (void)arg;
    s_auto_scan_count = count;
    s_auto_scan_aps = aps;
    xSemaphoreGive(s_auto_scan_sem);
}

static void _app_wifi_task(void *arg)
{
    plat_wifi_dev_t *wifi = impl_wifi_dev();
    plat_wifi_dev_init(wifi);
    plat_wifi_register_event_cb(wifi, _wifi_state_cb, NULL);
    // 上电自动连接：扫描 → 匹配已记住 SSID → 连接
    s_auto_scan_sem = xSemaphoreCreateBinary();
    if (s_auto_scan_sem)
    {
        plat_wifi_ap_info_t aps[WIFI_SCAN_MAX_AP];
        s_auto_scan_aps = aps;
        s_auto_scan_count = 0;

        // 注册临时 scan_cb 等待异步扫描结果
        plat_wifi_register_scan_cb(wifi, _auto_scan_cb, NULL);

        if (plat_wifi_scan(wifi, aps, WIFI_SCAN_MAX_AP) == 0)
        {
            // 阻塞等待 SCAN_DONE 事件回调（最多 10s）
            if (xSemaphoreTake(s_auto_scan_sem, pdMS_TO_TICKS(10000)) == pdTRUE)
            {
                int n = s_auto_scan_count;
                if (n > 0)
                {
                    int cred_count = 0;
                    const wifi_cred_t *creds = app_wifi_store_get_all(&cred_count);
                    for (int i = 0; i < n; i++)
                    {
                        for (int j = 0; j < cred_count; j++)
                        {
                            if (strcmp(aps[i].ssid, creds[j].ssid) == 0)
                            {
                                EK_LOG_INFO("auto-connecting to saved wifi: %s", creds[j].ssid);
                                plat_wifi_sta_start(wifi, creds[j].ssid, creds[j].password);
                                goto auto_connect_done;
                            }
                        }
                    }
                    EK_LOG_INFO("no saved wifi found in scan results");
                }
                else
                {
                    EK_LOG_INFO("auto-scan found no APs, skip auto-connect");
                }
            }
            else
            {
                EK_LOG_WARN("auto-scan timeout waiting for results");
            }
        }
        else
        {
            EK_LOG_WARN("auto-scan start failed");
        }

    auto_connect_done:
        // 取消临时 scan_cb，避免影响后续命令队列的扫描路径
        plat_wifi_register_scan_cb(wifi, NULL, NULL);
        vSemaphoreDelete(s_auto_scan_sem);
        s_auto_scan_sem = NULL;
    }

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
                        for (int i = 0; i < APP_WIFI_EVT_CB_MAX; i++)
                        {
                            if (s_evt_cbs[i])
                            {
                                lv_lock();
                                s_evt_cbs[i](APP_WIFI_EVT_SCAN_FAIL, NULL, s_evt_cb_args[i]);
                                lv_unlock();
                            }
                        }
                        break;
                    }

                    plat_wifi_register_scan_cb(wifi, _scan_result_cb, result);
                    if (plat_wifi_scan(wifi, result->aps, WIFI_SCAN_MAX_AP) != 0)
                    {
                        plat_wifi_register_scan_cb(wifi, NULL, NULL);
                        free(result);
                        for (int i = 0; i < APP_WIFI_EVT_CB_MAX; i++)
                        {
                            if (s_evt_cbs[i])
                            {
                                lv_lock();
                                s_evt_cbs[i](APP_WIFI_EVT_SCAN_FAIL, NULL, s_evt_cb_args[i]);
                                lv_unlock();
                            }
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
                EK_LOG_WARN("unknown wifi cmd: %d", msg.cmd);
                break;
        }
    }
}

bool app_wifi_is_connected(void)
{
    return plat_wifi_get_state(impl_wifi_dev()) == PLAT_WIFI_CONNECTED;
}
