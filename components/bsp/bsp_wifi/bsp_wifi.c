#include "bsp_wifi.h"

FILE_TAG("bsp_wifi.c");

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
#define WIFI_ESPTOUCH_BIT  BIT2

static void _ap_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static void _sta_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static void _smart_confi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static void _smart_config_task(void *arg);

static EventGroupHandle_t g_wifi_event_group_handler;
static esp_netif_t *g_netif;

void bsp_wifi_sta_init(const bsp_wifi_config_t *sta_cfg)
{
    assert(sta_cfg);

    g_wifi_event_group_handler = xEventGroupCreate();
    assert(g_wifi_event_group_handler);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    g_netif = esp_netif_create_default_wifi_sta();
    assert(g_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t any_id;
    esp_event_handler_instance_t got_id;

    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, _sta_event_handler, NULL, &any_id));
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, _sta_event_handler, NULL, &got_id));

    wifi_config_t wifi_cfg = { 0 };

    memcpy(wifi_cfg.sta.ssid, sta_cfg->ssid, 32);
    memcpy(wifi_cfg.sta.password, sta_cfg->pswd, 64);
    wifi_cfg.sta.threshold.authmode = sta_cfg->auth_mode;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());

    LOG_INFO("bsp wifi stattion mode init ok");
    LOG_INFO("start connecting wifi :\n\tssid:%s\n\tpassword:%s", sta_cfg->ssid, sta_cfg->pswd);

    EventBits_t bits = xEventGroupWaitBits(
        g_wifi_event_group_handler, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT)
    {
        LOG_INFO("successfully connect to the wifi");
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        LOG_WARN("fail to connect to the wif");
    }
    else
    {
        LOG_ERROR("unexpected event");
    }
}

void bsp_wifi_ap_init(const bsp_wifi_config_t *ap_cfg, uint8_t max_connections)
{
    assert(ap_cfg);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    (esp_netif_create_default_wifi_ap());

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, _ap_event_handler, NULL, NULL));

    // 配置热点相关信息
    wifi_config_t wifi_cfg = { 0 };

    memcpy(wifi_cfg.sta.ssid, ap_cfg->ssid, 32);
    memcpy(wifi_cfg.sta.password, ap_cfg->pswd, 64);
    wifi_cfg.ap.ssid_len = strlen((const char *)ap_cfg->ssid);
    wifi_cfg.ap.authmode = ap_cfg->auth_mode;
    wifi_cfg.ap.max_connection = max_connections;

    // 如果没有密码说明是开放热点
    if (strlen((const char *)ap_cfg->pswd) == 0) wifi_cfg.ap.authmode = WIFI_AUTH_OPEN;

    // 配置wifi一些信息
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());
    LOG_INFO("bsp wifi ap mode init ok\n\tssid:%s\n\tpassword:%s", ap_cfg->ssid, ap_cfg->pswd);
}

void bsp_wifi_smartconfig_init(const bsp_wifi_config_t *smt_cfg)
{
    ESP_ERROR_CHECK(esp_netif_init());
    g_wifi_event_group_handler = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, _smart_confi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, _smart_confi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, _smart_confi_event_handler, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    LOG_INFO("bsp wifi smart config mode init ok");
}

uint16_t bsp_wifi_scan(wifi_ap_record_t *ap_info, uint16_t scan_list_size)
{
    uint16_t ap_number = 0;
    // 开始扫描wifi(阻塞式)
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    // 获取扫描中AP数量
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_number));
    // 获取AP列表
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&scan_list_size, ap_info));

    return ap_number;
}

static void _ap_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        // 有设备连接
        wifi_event_ap_staconnected_t *info = (wifi_event_ap_staconnected_t *)event_data;
        LOG_INFO("station " MACSTR " join, AID=%d", MAC2STR(info->mac), info->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        // 设备断开连接
        wifi_event_ap_stadisconnected_t *info = (wifi_event_ap_stadisconnected_t *)event_data;
        LOG_INFO("station " MACSTR " disconnect, AID=%d", MAC2STR(info->mac), info->aid);
    }
}

static void _sta_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    static uint8_t retry_times;
    if (event_base == WIFI_EVENT)
    {
        if (event_id == WIFI_EVENT_STA_START)
        {
            esp_wifi_connect();
        }
        else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
        {
            // 没有连接成功，则尝试重连
            // 超过重连次数就设定为连接失败
            if (retry_times < BSP_WIFI_MAX_RETRY)
            {
                esp_wifi_connect();
                retry_times++;
                LOG_WARN("retry to connect to the wifi:%d", retry_times);
            }
            else
            {
                xEventGroupSetBits(g_wifi_event_group_handler, WIFI_FAIL_BIT);
            }
            LOG_WARN("fail to connect to the wifi");
        }
    }
    else if (event_base == IP_EVENT)
    {
        if (event_id == IP_EVENT_STA_GOT_IP)
        {
            // 被DHCP分配了IP才说明真正的连接成功
            // 给对应的事件组的对应位置1
            ip_event_got_ip_t *ip = (ip_event_got_ip_t *)event_data;
            LOG_INFO("successfully connected to the wifi");
            retry_times = 0;
            xEventGroupSetBits(g_wifi_event_group_handler, WIFI_CONNECTED_BIT);
        }
    }
}

static void _smart_confi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        // 开始陪网
        // 创建一个任务用于自动配置网
        xTaskCreate(_smart_config_task, "smart config task", 4096, NULL, 3, NULL);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        // 断开链接
        esp_wifi_connect();
        xEventGroupClearBits(g_wifi_event_group_handler, WIFI_CONNECTED_BIT);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        // 被DCHDP 分配了IP地址
        // 说明确实连接好了
        xEventGroupSetBits(g_wifi_event_group_handler, WIFI_CONNECTED_BIT);
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE)
    {
        LOG_INFO("scan done");
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL)
    {
        LOG_INFO("found channel");
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD)
    {
        LOG_INFO("got ssid and password");

        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
        wifi_config_t wifi_config;
        uint8_t ssid[33] = { 0 };
        uint8_t password[65] = { 0 };
        uint8_t rvd_data[33] = { 0 };

        bzero(&wifi_config, sizeof(wifi_config_t));
        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));

        memcpy(ssid, evt->ssid, sizeof(evt->ssid));
        memcpy(password, evt->password, sizeof(evt->password));
        LOG_INFO("ssid:%s", ssid);
        LOG_INFO("password:%s", password);
        if (evt->type == SC_TYPE_ESPTOUCH_V2)
        {
            ESP_ERROR_CHECK(esp_smartconfig_get_rvd_data(rvd_data, sizeof(rvd_data)));
            LOG_INFO("rvd_data:");
            for (int i = 0; i < 33; i++)
            {
                printf("%02x ", rvd_data[i]);
            }
            printf("\n");
        }

        ESP_ERROR_CHECK(esp_wifi_disconnect());
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
        esp_wifi_connect();
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE)
    {
        xEventGroupSetBits(g_wifi_event_group_handler, WIFI_ESPTOUCH_BIT);
    }
}

static void _smart_config_task(void *arg)
{
    EventBits_t uxBits;
    esp_smartconfig_set_type(SC_TYPE_ESPTOUCH);
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    esp_smartconfig_start(&cfg);
    while (1)
    {
        uxBits = xEventGroupWaitBits(
            g_wifi_event_group_handler, WIFI_CONNECTED_BIT | WIFI_ESPTOUCH_BIT, true, false, portMAX_DELAY);

        if (uxBits & WIFI_CONNECTED_BIT) LOG_INFO("successfully connected to the wifi");
        if (uxBits & WIFI_ESPTOUCH_BIT)
        {
            LOG_INFO("smart config over");
            esp_smartconfig_stop();
            vTaskDelete(NULL);
        }
    }
}
