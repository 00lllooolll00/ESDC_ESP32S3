#include "common_header.h"
#include "app_weather_api.h"
#include "app_ip_location.h"
#include "app_wifi.h"
#include "app_weather_store.h"
#include "app_ai_advice.h"
#include "ek_export.h"

EK_LOG_FILE_TAG("app_weather_api");

#define APP_WEATHER_API_TASK_STACK     8192
#define APP_WEATHER_API_TASK_PRIO      3
#define APP_WEATHER_API_FIRST_DELAY_MS 30000 // 首延 30s 等 WiFi+IP 定位
#define APP_WEATHER_API_INTERVAL_MS    1800000 // 30 分钟
#define APP_WEATHER_API_WIFI_RETRY_MS  10000 // WiFi 未连时 10s 快速重试

extern const weather_provider_t g_qweather_provider;

static const weather_provider_t *s_provider = &g_qweather_provider;
static TaskHandle_t s_task = NULL;

static void _api_task(void *arg);
static void _wifi_evt_cb(app_wifi_evt_t evt, void *data, void *arg);

void app_weather_api_init(void)
{
    EK_LOG_INFO("ek_export: APP app_weather_api_init, provider=%s", s_provider ? s_provider->name : "(null)");
    BaseType_t ok =
        xTaskCreate(_api_task, "weather_api", APP_WEATHER_API_TASK_STACK, NULL, APP_WEATHER_API_TASK_PRIO, &s_task);
    assert(ok == pdPASS);
    app_wifi_register_evt_cb(_wifi_evt_cb, NULL);
}

EK_EXPORT_APP(app_weather_api_init, 5);

void app_weather_api_request(void)
{
    if (s_task)
    {
        xTaskNotifyGive(s_task);
    }
}

void app_weather_api_set_provider(const weather_provider_t *provider)
{
    s_provider = provider;
    EK_LOG_INFO("provider switched to %s", provider ? provider->name : "(null)");
}

// WiFi 事件回调：连接成功时唤醒天气 task 立即拉取，不用等轮询
static void _wifi_evt_cb(app_wifi_evt_t evt, void *data, void *arg)
{
    (void)data;
    (void)arg;
    if (evt == APP_WIFI_EVT_CONNECTED && s_task)
    {
        EK_LOG_INFO("weather api: wifi connected, notify task");
        xTaskNotifyGive(s_task);
    }
}

static void _api_task(void *arg)
{
    (void)arg;
    ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(APP_WEATHER_API_FIRST_DELAY_MS));

    while (1)
    {
        // 前置条件1：WiFi 必须已连接
        if (!app_wifi_is_connected())
        {
            EK_LOG_WARN("weather api: wifi not connected, retry in 10s");
            ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(APP_WEATHER_API_WIFI_RETRY_MS));
            continue;
        }

        weather_location_t loc = { 0 };
        app_ip_location_get_city(loc.city, sizeof(loc.city));
        app_ip_location_get_latlon(&loc.lat, &loc.lon);

        if (loc.lat != 0.0f && loc.lon != 0.0f && s_provider && s_provider->fetch)
        {
            EK_LOG_INFO("weather api: fetching via %s, loc=%.2f,%.2f", s_provider->name, loc.lon, loc.lat);
            app_weather_forecast_t fc = { 0 };
            if (s_provider->fetch(&loc, &fc))
            {
                if (fc.count > 0)
                {
                    app_weather_set_forecast(fc.temps, fc.count);
                }
                app_weather_set_current(fc.type, fc.current_temp, fc.humidity, fc.wind_speed);
            if (fc.daily_count > 0)
            {
                app_weather_set_daily(fc.dailies, fc.daily_count);
            }
                EK_LOG_INFO("weather api: data updated");
                app_weather_store_save(&fc);
                // AI 建议（DeepSeek）
                if (app_ai_advice_request(&fc, fc.ai_advice, sizeof(fc.ai_advice)))
                {
                    app_weather_set_ai_advice(fc.ai_advice);
                }
            }
            else
            {
                EK_LOG_WARN("weather api: fetch failed, retry in 30s");
                // 网络偶发超时快速重试，不等 30 分钟
                ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(30000));
                continue;
            }
        }
        else
        {
            EK_LOG_WARN("weather api: no location, waiting for IP location...");
            // 等待 IP 定位完成的通知（15s 超时），不进入 30 分钟长等待
            ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(15000));
            continue;
        }

        // 等 30 分钟或被 request 唤醒
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(APP_WEATHER_API_INTERVAL_MS));
    }
}
