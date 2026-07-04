#include "common_header.h"
#include <string.h>

#include "app_weather.h"
#include "ek_export.h"
#include "lvgl.h"
#include "cJSON.h"

EK_LOG_FILE_TAG("app_weather");

static app_weather_forecast_t s_forecast;
static SemaphoreHandle_t s_mutex;
static app_weather_ui_cb_t s_ui_cb;
static void *s_ui_cb_arg;

static const char *const s_type_names[] = {
    [APP_WEATHER_TYPE_UNKNOWN] = "未知", [APP_WEATHER_TYPE_SUNNY] = "晴", [APP_WEATHER_TYPE_CLOUDY] = "多云",
    [APP_WEATHER_TYPE_OVERCAST] = "阴",  [APP_WEATHER_TYPE_RAIN] = "雨",  [APP_WEATHER_TYPE_THUNDER] = "雷雨",
    [APP_WEATHER_TYPE_SNOW] = "雪",
};

// 解析天气类型字符串为枚举
static app_weather_type_t _parse_type(const char *s)
{
    if (!s)
    {
        return APP_WEATHER_TYPE_UNKNOWN;
    }
    if (strcmp(s, "sunny") == 0)
    {
        return APP_WEATHER_TYPE_SUNNY;
    }
    if (strcmp(s, "cloudy") == 0)
    {
        return APP_WEATHER_TYPE_CLOUDY;
    }
    if (strcmp(s, "overcast") == 0)
    {
        return APP_WEATHER_TYPE_OVERCAST;
    }
    if (strcmp(s, "rain") == 0)
    {
        return APP_WEATHER_TYPE_RAIN;
    }
    if (strcmp(s, "thunder") == 0)
    {
        return APP_WEATHER_TYPE_THUNDER;
    }
    if (strcmp(s, "snow") == 0)
    {
        return APP_WEATHER_TYPE_SNOW;
    }
    return APP_WEATHER_TYPE_UNKNOWN;
}

// 在持锁状态下从 temps 派生 max/min
static void _derive_max_min(void)
{
    if (s_forecast.count <= 0)
    {
        return;
    }
    int16_t mx = s_forecast.temps[0];
    int16_t mn = s_forecast.temps[0];
    for (int i = 1; i < s_forecast.count; i++)
    {
        if (s_forecast.temps[i] > mx)
        {
            mx = s_forecast.temps[i];
        }
        if (s_forecast.temps[i] < mn)
        {
            mn = s_forecast.temps[i];
        }
    }
    s_forecast.max_temp = mx;
    s_forecast.min_temp = mn;
}

// 释放锁后触发 UI 回调（lv_lock 保护 LVGL 操作）
static void _notify_ui(void)
{
    if (s_ui_cb)
    {
        lv_lock();
        s_ui_cb(&s_forecast, s_ui_cb_arg);
        lv_unlock();
    }
}

void app_weather_init(void)
{
    EK_LOG_INFO("ek_export: APP app_weather_init");
    s_mutex = xSemaphoreCreateMutex();
    assert(s_mutex);
    memset(&s_forecast, 0, sizeof(s_forecast));
    s_forecast.type = APP_WEATHER_TYPE_UNKNOWN;
    s_ui_cb = NULL;
    s_ui_cb_arg = NULL;
}

EK_EXPORT_APP(app_weather_init, 2);

void app_weather_set_forecast(const int16_t *temps, int count)
{
    if (!s_mutex || !temps || count <= 0)
    {
        return;
    }

    // 截断到最大点数
    if (count > APP_WEATHER_MAX_POINTS)
    {
        count = APP_WEATHER_MAX_POINTS;
    }

    // 互斥锁保护存储
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    memcpy(s_forecast.temps, temps, sizeof(int16_t) * count);
    s_forecast.count = count;
    _derive_max_min();
    xSemaphoreGive(s_mutex);

    EK_LOG_INFO("forecast updated, count=%d", count);

    // 触发 UI 回调
    _notify_ui();
}

int app_weather_inject_json(const char *json_str)
{
    if (!json_str)
    {
        return -1;
    }

    cJSON *root = cJSON_Parse(json_str);
    if (!root)
    {
        EK_LOG_WARN("json parse failed: %s", json_str);
        return -1;
    }

    cJSON *temps_arr = cJSON_GetObjectItem(root, "temps");
    if (!temps_arr || !cJSON_IsArray(temps_arr))
    {
        EK_LOG_WARN("missing \"temps\" array in json");
        cJSON_Delete(root);
        return -1;
    }

    int arr_size = cJSON_GetArraySize(temps_arr);
    if (arr_size <= 0)
    {
        EK_LOG_WARN("empty temps array");
        cJSON_Delete(root);
        return -1;
    }

    // 截断到最大点数
    if (arr_size > APP_WEATHER_MAX_POINTS)
    {
        arr_size = APP_WEATHER_MAX_POINTS;
    }

    int16_t temps[APP_WEATHER_MAX_POINTS];
    for (int i = 0; i < arr_size; i++)
    {
        cJSON *item = cJSON_GetArrayItem(temps_arr, i);
        // 摄氏度浮点 ×10 转 int16_t（如 25.5 → 255）
        double val = cJSON_GetNumberValue(item);
        temps[i] = (int16_t)(val * 10.0 + 0.5);
    }

    // 解析可选字段（root 仍有效）
    app_weather_type_t type = APP_WEATHER_TYPE_UNKNOWN;
    int16_t current_temp = 0;
    uint8_t humidity = 0;
    uint8_t wind_speed = 0;

    cJSON *type_item = cJSON_GetObjectItem(root, "type");
    if (type_item && cJSON_IsString(type_item))
    {
        type = _parse_type(type_item->valuestring);
    }

    cJSON *cur_item = cJSON_GetObjectItem(root, "current_temp");
    if (cur_item)
    {
        current_temp = (int16_t)(cJSON_GetNumberValue(cur_item) * 10.0 + 0.5);
    }

    cJSON *hum_item = cJSON_GetObjectItem(root, "humidity");
    if (hum_item)
    {
        humidity = (uint8_t)cJSON_GetNumberValue(hum_item);
    }

    cJSON *wind_item = cJSON_GetObjectItem(root, "wind_speed");
    if (wind_item)
    {
        wind_speed = (uint8_t)cJSON_GetNumberValue(wind_item);
    }

    cJSON_Delete(root);

    // 先设预报（含 temps 派生 max/min），再设当前状态
    app_weather_set_forecast(temps, arr_size);
    app_weather_set_current(type, current_temp, humidity, wind_speed);
    return 0;
}

void app_weather_register_ui_cb(app_weather_ui_cb_t cb, void *arg)
{
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    s_ui_cb = cb;
    s_ui_cb_arg = arg;
    xSemaphoreGive(s_mutex);
}

app_weather_forecast_t app_weather_get_forecast(void)
{
    app_weather_forecast_t snapshot;
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    snapshot = s_forecast;
    xSemaphoreGive(s_mutex);
    return snapshot;
}

void app_weather_set_current(app_weather_type_t type, int16_t current_temp, uint8_t humidity, uint8_t wind_speed)
{
    if (!s_mutex)
    {
        return;
    }

    xSemaphoreTake(s_mutex, portMAX_DELAY);
    s_forecast.type = type;
    s_forecast.current_temp = current_temp;
    s_forecast.humidity = humidity;
    s_forecast.wind_speed = wind_speed;
    xSemaphoreGive(s_mutex);

    EK_LOG_INFO("current weather updated: type=%d temp=%d hum=%d wind=%d", type, current_temp, humidity, wind_speed);

    // 触发 UI 回调
    _notify_ui();
}

void app_weather_set_daily(const app_weather_daily_t *dailies, int count)
{
    if (!s_mutex || !dailies || count <= 0)
    {
        return;
    }
    if (count > APP_WEATHER_DAILY_MAX)
    {
        count = APP_WEATHER_DAILY_MAX;
    }
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    s_forecast.daily_count = count;
    for (int i = 0; i < count; i++)
    {
        s_forecast.dailies[i] = dailies[i];
    }
    xSemaphoreGive(s_mutex);
    EK_LOG_INFO("daily forecast updated: count=%d", count);
    _notify_ui();
}

void app_weather_set_ai_advice(const char *advice)
{
    if (!s_mutex || !advice)
    {
        return;
    }
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    strncpy(s_forecast.ai_advice, advice, sizeof(s_forecast.ai_advice) - 1);
    s_forecast.ai_advice[sizeof(s_forecast.ai_advice) - 1] = '\0';
    xSemaphoreGive(s_mutex);
    EK_LOG_INFO("ai advice updated: %s", s_forecast.ai_advice);
    _notify_ui();
}

const char *app_weather_type_name(app_weather_type_t t)
{
    if (t < 0 || t >= (int)(sizeof(s_type_names) / sizeof(s_type_names[0])))
    {
        return s_type_names[APP_WEATHER_TYPE_UNKNOWN];
    }
    return s_type_names[t];
}

const char *app_weather_type_icon_name(app_weather_type_t t)
{
    switch (t)
    {
    case APP_WEATHER_TYPE_SUNNY:    return "S:/images/weather_sunny.bin";
    case APP_WEATHER_TYPE_CLOUDY:   return "S:/images/weather_cloudy.bin";
    case APP_WEATHER_TYPE_OVERCAST: return "S:/images/weather_overcast.bin";
    case APP_WEATHER_TYPE_RAIN:     return "S:/images/weather_rain.bin";
    case APP_WEATHER_TYPE_THUNDER:  return "S:/images/weather_thunder.bin";
    case APP_WEATHER_TYPE_SNOW:     return "S:/images/weather_snow.bin";
    default:                        return "S:/images/weather_unknown.bin";
    }
}
