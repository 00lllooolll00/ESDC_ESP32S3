#include "impl_wifi.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_mac.h"
#include "esp_netif.h"

FILE_TAG("impl_wifi");

#define IMPL_WIFI_MAX_RETRY 5

static plat_wifi_dev_t *s_wifi_dev;

static int _wifi_dev_init(void);
static int _wifi_dev_deinit(void);
static int _wifi_dev_suspend(void);
static int _wifi_dev_resume(void);

static const plat_dev_ops_t s_wifi_base_ops = {
    .init    = _wifi_dev_init,
    .deinit  = _wifi_dev_deinit,
    .suspend = _wifi_dev_suspend,
    .resume  = _wifi_dev_resume,
};

static int _wifi_sta_start(const char *ssid, const char *passwd);
static int _wifi_sta_stop(void);
static int _wifi_scan(plat_wifi_ap_info_t *ap_info, uint16_t max_count);

static const plat_wifi_ops_t s_wifi_ops = {
    .sta_start = _wifi_sta_start,
    .sta_stop   = _wifi_sta_stop,
    .scan       = _wifi_scan,
};

static void _wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static uint8_t s_retry_count;

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

static int _wifi_sta_start(const char *ssid, const char *passwd)
{
    wifi_config_t wifi_cfg = {0};
    strlcpy((char *)wifi_cfg.sta.ssid, ssid, sizeof(wifi_cfg.sta.ssid));
    if (passwd) strlcpy((char *)wifi_cfg.sta.password, passwd, sizeof(wifi_cfg.sta.password));
    wifi_cfg.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());

    LOG_INFO("wifi sta start: ssid=%s", ssid);
    return 0;
}

static int _wifi_sta_stop(void)
{
    ESP_ERROR_CHECK(esp_wifi_disconnect());
    ESP_ERROR_CHECK(esp_wifi_stop());
    return 0;
}

static int _wifi_scan(plat_wifi_ap_info_t *ap_info, uint16_t max_count)
{
    uint16_t ap_number = 0;
    wifi_ap_record_t *records = malloc(sizeof(wifi_ap_record_t) * max_count);
    if (!records) return -1;

    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_number));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&max_count, records));

    for (int i = 0; i < max_count && i < ap_number; i++)
    {
        strlcpy(ap_info[i].ssid, (char *)records[i].ssid, sizeof(ap_info[i].ssid));
        memcpy(ap_info[i].bssid, records[i].bssid, 6);
        ap_info[i].rssi = records[i].rssi;
        ap_info[i].auth_mode = records[i].authmode;
    }

    free(records);
    return max_count < ap_number ? max_count : ap_number;
}

static void _wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT)
    {
        if (event_id == WIFI_EVENT_STA_START)
        {
            esp_wifi_connect();
        }
        else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
        {
            if (s_retry_count < IMPL_WIFI_MAX_RETRY)
            {
                esp_wifi_connect();
                s_retry_count++;
                LOG_WARN("wifi retry connect: %d", s_retry_count);
            }
            else
            {
                LOG_WARN("wifi connect failed after %d retries", IMPL_WIFI_MAX_RETRY);
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