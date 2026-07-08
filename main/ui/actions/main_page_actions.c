#include "common_header.h"
#include "ui.h"
#include "app_wifi.h"
#include "lvgl.h"
#include "esp_sntp.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include <time.h>
#include <sys/time.h>
#include "nvs.h"

EK_LOG_FILE_TAG("main_page_actions");

#define TIME_STORE_NVS_NS  "ek_time"
#define TIME_STORE_NVS_KEY "ts"
#define TIME_STORE_TS_MIN  1700000000 // 2023 年时间戳下限，小于此值视为无效

static lv_timer_t *s_clock_timer;

// 时间可用标志：NVS 恢复或 SNTP 同步后置 true，不受 sntp_get_sync_status() 一次性重置影响
static volatile bool s_time_ready = false;

// 从 NVS 恢复上次保存的时间戳，设为系统时间（WiFi 未连也先有时钟显示）
static void _restore_stored_time(void)
{
    nvs_handle_t h;
    if (nvs_open(TIME_STORE_NVS_NS, NVS_READONLY, &h) != ESP_OK)
    {
        return;
    }
    int64_t stored_ts = 0;
    esp_err_t err = nvs_get_i64(h, TIME_STORE_NVS_KEY, &stored_ts);
    nvs_close(h);
    if (err != ESP_OK || stored_ts < (int64_t)TIME_STORE_TS_MIN)
    {
        return;
    }
    struct timeval tv = { .tv_sec = (time_t)stored_ts, .tv_usec = 0 };
    settimeofday(&tv, NULL);
    s_time_ready = true;
    EK_LOG_INFO("restored time from NVS: unix timestamp=%lld", (long long)stored_ts);
}

// SNTP 时间同步回调：每次 SNTP 成功同步时由 lwip 线程调用
static void _sntp_sync_cb(struct timeval *tv)
{
    s_time_ready = true;
    EK_LOG_INFO("SNTP time synced, unix timestamp=%lld", (long long)tv->tv_sec);
    // 持久化到 NVS 供下次上电使用
    nvs_handle_t h;
    if (nvs_open(TIME_STORE_NVS_NS, NVS_READWRITE, &h) == ESP_OK)
    {
        nvs_set_i64(h, TIME_STORE_NVS_KEY, (int64_t)tv->tv_sec);
        nvs_commit(h);
        nvs_close(h);
    }
}

// 时钟更新回调：每秒刷新时间和日期
static void _clock_timer_cb(lv_timer_t *t)
{
    (void)t;

    // 时间未就绪时不显示（NVS 恢复或 SNTP 同步后才有值）
    if (!s_time_ready)
    {
        return;
    }

    time_t now = time(NULL);
    struct tm *tm = localtime(&now);

    // 时间 "HH:MM"
    char time_buf[16];
    snprintf(time_buf, sizeof(time_buf), "%02d:%02d", tm->tm_hour, tm->tm_min);
    lv_obj_t *clock_lbl = lv_obj_find_by_name(main_page, "main_clock");
    if (clock_lbl)
    {
        lv_label_set_text(clock_lbl, time_buf);
    }

    // 日期 "M月D日 周X"
    static const char *weekdays[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
    char date_buf[32];
    snprintf(date_buf, sizeof(date_buf), "%d月%d日 %s",
             tm->tm_mon + 1, tm->tm_mday, weekdays[tm->tm_wday]);
    lv_obj_t *date_lbl = lv_obj_find_by_name(main_page, "main_date");
    if (date_lbl)
    {
        lv_label_set_text(date_lbl, date_buf);
    }
}

// 更新底部状态栏
static void _update_status_bar(void)
{
    // WiFi SSID
    wifi_ap_record_t ap_info;
    char ssid_buf[40] = "WiFi: --";
    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK && ap_info.ssid[0])
    {
        snprintf(ssid_buf, sizeof(ssid_buf), "WiFi: %s", (char *)ap_info.ssid);
    }
    lv_obj_t *ssid_lbl = lv_obj_find_by_name(main_page, "main_wifi_ssid");
    if (ssid_lbl)
    {
        lv_label_set_text(ssid_lbl, ssid_buf);
    }

    // IP 地址
    char ip_buf[24] = "IP: --";
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif)
    {
        esp_netif_ip_info_t ip_info;
        if (esp_netif_get_ip_info(netif, &ip_info) == ESP_OK)
        {
            snprintf(ip_buf, sizeof(ip_buf), "IP: " IPSTR, IP2STR(&ip_info.ip));
        }
    }
    lv_obj_t *ip_lbl = lv_obj_find_by_name(main_page, "main_ip_addr");
    if (ip_lbl)
    {
        lv_label_set_text(ip_lbl, ip_buf);
    }
}

// WiFi 事件回调：连接成功时更新状态栏
static void _wifi_evt_cb(app_wifi_evt_t evt, void *data, void *arg)
{
    (void)data;
    (void)arg;
    if (evt == APP_WIFI_EVT_CONNECTED)
    {
        _update_status_bar();
    }
}

void main_page_actions_init(void)
{
    // 设时区 CST-8
    setenv("TZ", "CST-8", 1);
    tzset();

    // 从 NVS 恢复上次保存的时间戳（WiFi 未连也先有时钟显示）
    _restore_stored_time();

    // 启动 SNTP（WiFi 连上后自动同步并更新时间）
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_set_time_sync_notification_cb(_sntp_sync_cb);
    esp_sntp_init();

    // 启动时钟定时器（每秒更新）
    s_clock_timer = lv_timer_create(_clock_timer_cb, 1000, NULL);

    // 注册 WiFi 事件回调，连接成功时更新状态栏
    app_wifi_register_evt_cb(_wifi_evt_cb, NULL);

    // 立即刷新一次状态栏（可能 WiFi 还没连，显示默认值）
    _update_status_bar();

    EK_LOG_INFO("main page actions initialized");
}
