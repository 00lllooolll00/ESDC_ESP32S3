#include "impl_wifi.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "freertos/semphr.h"

FILE_TAG("impl_wifi");

#define IMPL_WIFI_MAX_RETRY 5

static plat_wifi_dev_t *s_wifi_dev;
static plat_wifi_ap_info_t *s_scan_ap_info;
static uint16_t s_scan_max_count;

static int _wifi_dev_init(void);
static int _wifi_dev_deinit(void);
static int _wifi_dev_suspend(void);
static int _wifi_dev_resume(void);

static const plat_dev_ops_t s_wifi_base_ops = {
    .init = _wifi_dev_init,
    .deinit = _wifi_dev_deinit,
    .suspend = _wifi_dev_suspend,
    .resume = _wifi_dev_resume,
};

static int _wifi_sta_start(const char *ssid, const char *passwd);
static int _wifi_sta_stop(void);
static int _wifi_scan(plat_wifi_ap_info_t *ap_info, uint16_t max_count);

static const plat_wifi_ops_t s_wifi_ops = {
    .sta_start = _wifi_sta_start,
    .sta_stop = _wifi_sta_stop,
    .scan = _wifi_scan,
};

static void _wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static uint8_t s_retry_count;
static bool s_wifi_started;
static SemaphoreHandle_t s_wifi_start_sem;

int impl_wifi_register(plat_wifi_dev_t *wifi_dev)
{
    plat_wifi_dev_register(wifi_dev, "wifi", &s_wifi_base_ops, &s_wifi_ops, NULL);
    s_wifi_dev = wifi_dev;
    return 0;
}

static int _wifi_dev_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *netif = esp_netif_create_default_wifi_sta();
    assert(netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    s_wifi_start_sem = xSemaphoreCreateBinary();
    assert(s_wifi_start_sem);

    esp_event_handler_instance_t wifi_instance;
    esp_event_handler_instance_t ip_instance;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, _wifi_event_handler, NULL, &wifi_instance));
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, _wifi_event_handler, NULL, &ip_instance));

    return 0;
}

static int _wifi_dev_deinit(void)
{
    if (s_wifi_started) {
        esp_wifi_stop();
        s_wifi_started = false;
    }
    if (s_wifi_start_sem) {
        vSemaphoreDelete(s_wifi_start_sem);
        s_wifi_start_sem = NULL;
    }
    return 0;
}

static int _wifi_dev_suspend(void)
{
    return 0;
}

static int _wifi_dev_resume(void)
{
    return 0;
}

static int _wifi_ensure_started(void)
{
    if (s_wifi_started) return 0;

    ESP_ERROR_CHECK(esp_wifi_start());
    if (xSemaphoreTake(s_wifi_start_sem, pdMS_TO_TICKS(5000)) != pdTRUE) {
        LOG_ERROR("wifi start timeout");
        return -1;
    }
    s_wifi_started = true;
    LOG_INFO("wifi started (on-demand)");
    return 0;
}

static int _wifi_sta_start(const char *ssid, const char *passwd)
{
    if (_wifi_ensure_started() != 0) return -1;

    s_retry_count = 0;

    wifi_config_t wifi_cfg = { 0 };
    strlcpy((char *)wifi_cfg.sta.ssid, ssid, sizeof(wifi_cfg.sta.ssid));
    if (passwd) strlcpy((char *)wifi_cfg.sta.password, passwd, sizeof(wifi_cfg.sta.password));
    wifi_cfg.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_connect());

    LOG_INFO("wifi sta start: ssid=%s", ssid);
    return 0;
}

static int _wifi_sta_stop(void)
{
    ESP_ERROR_CHECK(esp_wifi_disconnect());
    ESP_ERROR_CHECK(esp_wifi_stop());
    s_wifi_started = false;
    s_retry_count = IMPL_WIFI_MAX_RETRY;
    return 0;
}

static int _wifi_scan(plat_wifi_ap_info_t *ap_info, uint16_t max_count)
{
    if (_wifi_ensure_started() != 0) return -1;

    s_scan_ap_info = ap_info;
    s_scan_max_count = max_count;

    esp_err_t err = esp_wifi_scan_start(NULL, false);
    if (err != ESP_OK)
    {
        LOG_ERROR("wifi scan start failed: %s", unified_strerror(err));
        return -1;
    }
    return 0;
}

static void _wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT)
    {
        if (event_id == WIFI_EVENT_SCAN_DONE)
        {
            uint16_t ap_number = 0;
            esp_wifi_scan_get_ap_num(&ap_number);

            uint16_t rec_count = s_scan_max_count;
            wifi_ap_record_t *records = malloc(sizeof(wifi_ap_record_t) * rec_count);
            if (!records) return;

            esp_err_t err = esp_wifi_scan_get_ap_records(&rec_count, records);
            if (err == ESP_OK)
            {
                for (int i = 0; i < rec_count && i < ap_number; i++)
                {
                    strlcpy(s_scan_ap_info[i].ssid, (char *)records[i].ssid, sizeof(s_scan_ap_info[i].ssid));
                    memcpy(s_scan_ap_info[i].bssid, records[i].bssid, 6);
                    s_scan_ap_info[i].rssi = records[i].rssi;
                    s_scan_ap_info[i].auth_mode = records[i].authmode;
                }
            }

            free(records);

            int result_count = (err == ESP_OK) ? ((rec_count < ap_number) ? rec_count : ap_number) : -1;
            if (s_wifi_dev) plat_wifi_notify_scan_result(s_wifi_dev, result_count, s_scan_ap_info);
        }
        else if (event_id == WIFI_EVENT_STA_START)
        {
            xSemaphoreGive(s_wifi_start_sem);
        }
        else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
        {
            wifi_event_sta_disconnected_t *disconn = (wifi_event_sta_disconnected_t *)event_data;

            // 认证失败（密码错误等）不重试，立即通知上层
            if (disconn->reason == WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT || disconn->reason == WIFI_REASON_AUTH_FAIL)
            {
                LOG_WARN("wifi auth failed, reason=%d", disconn->reason);
                if (s_wifi_dev) plat_wifi_notify_state(s_wifi_dev, PLAT_WIFI_DISCONNECTED);
            }
            else if (s_retry_count < IMPL_WIFI_MAX_RETRY)
            {
                esp_wifi_connect();
                s_retry_count++;
                LOG_WARN("wifi retry connect: %d, reason=%d", s_retry_count, disconn->reason);
            }
            else
            {
                LOG_WARN("wifi connect failed after %d retries, reason=%d", IMPL_WIFI_MAX_RETRY, disconn->reason);
                if (s_wifi_dev) plat_wifi_notify_state(s_wifi_dev, PLAT_WIFI_DISCONNECTED);
            }
        }
    }
    else if (event_base == IP_EVENT)
    {
        if (event_id == IP_EVENT_STA_GOT_IP)
        {
            ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
            LOG_INFO("wifi got ip: " IPSTR, IP2STR(&event->ip_info.ip));
            s_retry_count = 0;
            if (s_wifi_dev) plat_wifi_notify_state(s_wifi_dev, PLAT_WIFI_CONNECTED);
        }
    }
}
