#include "common_header.h"

#include "app_weather_store.h"
#include "app_weather.h"
#include "ek_export.h"
#include "nvs.h"
#include <time.h>
#include <string.h>

EK_LOG_FILE_TAG("app_weather_store");

#define WEATHER_STORE_NVS_NS  "ek_weather"
#define WEATHER_STORE_NVS_KEY "fc_cache"
#define WEATHER_STORE_MAGIC   0x57435432 // "WCT2"
#define WEATHER_STORE_TS_MIN  1700000000 // 2023 年时间戳下限，小于此值视为 SNTP 未同步

// 持久化结构：magic 校验 + 时间戳 + 天气数据，整体作为 NVS blob 存取
typedef struct
{
    uint32_t magic;
    time_t timestamp;
    app_weather_forecast_t fc;
} weather_store_t;

void app_weather_store_init(void)
{
    nvs_handle_t h;
    esp_err_t err = nvs_open(WEATHER_STORE_NVS_NS, NVS_READONLY, &h);
    if (err != ESP_OK)
    {
        EK_LOG_WARN("weather store: no cached data (nvs_open: %s)", unified_strerror(err));
        return;
    }

    weather_store_t store = { 0 };
    size_t required = sizeof(store);
    err = nvs_get_blob(h, WEATHER_STORE_NVS_KEY, &store, &required);
    nvs_close(h);

    if (err != ESP_OK)
    {
        EK_LOG_WARN("weather store: no cached data (nvs_get_blob: %s)", unified_strerror(err));
        return;
    }

    if (required != sizeof(store) || store.magic != WEATHER_STORE_MAGIC)
    {
        EK_LOG_WARN("weather store: cached data invalid (size=%u, magic=0x%08x)", (unsigned)required, store.magic);
        return;
    }

    // 注入缓存数据到 app_weather 模块
    // 此时 s_ui_cb 为 NULL（weather_ui_init level 4 才注册），_notify_ui 不会触发回调
    if (store.fc.count > 0)
    {
        app_weather_set_forecast(store.fc.temps, store.fc.count);
    }
    app_weather_set_current(store.fc.type, store.fc.current_temp, store.fc.humidity, store.fc.wind_speed);
    if (store.fc.daily_count > 0)
    {
        app_weather_set_daily(store.fc.dailies, store.fc.daily_count);
    }
    if (strlen(store.fc.ai_advice) > 0)
    {
        app_weather_set_ai_advice(store.fc.ai_advice);
    }

    if (store.timestamp >= (time_t)WEATHER_STORE_TS_MIN)
    {
        EK_LOG_INFO("weather store: loaded cached data, timestamp=%ld", (long)store.timestamp);
    }
    else
    {
        EK_LOG_INFO("weather store: loaded cached data (timestamp invalid, sntp not synced)");
    }
}

void app_weather_store_save(const app_weather_forecast_t *fc)
{
    if (!fc)
    {
        return;
    }

    weather_store_t store = { 0 };
    store.magic = WEATHER_STORE_MAGIC;
    store.fc = *fc;

    // SNTP 同步后 time(NULL) 返回真实 Unix 时间戳；未同步时返回值很小
    time_t now = time(NULL);
    store.timestamp = (now >= (time_t)WEATHER_STORE_TS_MIN) ? now : 0;

    nvs_handle_t h;
    esp_err_t err = nvs_open(WEATHER_STORE_NVS_NS, NVS_READWRITE, &h);
    if (err != ESP_OK)
    {
        EK_LOG_WARN("weather store: save failed (nvs_open: %s)", unified_strerror(err));
        return;
    }

    err = nvs_set_blob(h, WEATHER_STORE_NVS_KEY, &store, sizeof(store));
    if (err != ESP_OK)
    {
        EK_LOG_WARN("weather store: save failed (nvs_set_blob: %s)", unified_strerror(err));
    }
    else
    {
        nvs_commit(h);
        EK_LOG_INFO("weather store: saved, timestamp=%ld", (long)store.timestamp);
    }
    nvs_close(h);
}

EK_EXPORT_APP(app_weather_store_init, 3);
