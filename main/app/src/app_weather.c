#include "app_weather.h"
#include "ek_export.h"
#include "lvgl.h"

#include "cJSON.h"
#include <string.h>

FILE_TAG("app_weather");

static app_weather_forecast_t s_forecast;
static SemaphoreHandle_t s_mutex;
static app_weather_ui_cb_t s_ui_cb;
static void *s_ui_cb_arg;

void app_weather_init(void)
{
    LOG_INFO("ek_export: APP app_weather_init");
    s_mutex = xSemaphoreCreateMutex();
    assert(s_mutex);
    s_forecast.count = 0;
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
    xSemaphoreGive(s_mutex);

    LOG_INFO("forecast updated, count=%d", count);

    // 触发 UI 回调，lv_lock 保护 LVGL 操作
    if (s_ui_cb)
    {
        lv_lock();
        s_ui_cb(&s_forecast, s_ui_cb_arg);
        lv_unlock();
    }
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
        LOG_WARN("json parse failed: %s", json_str);
        return -1;
    }

    cJSON *temps_arr = cJSON_GetObjectItem(root, "temps");
    if (!temps_arr || !cJSON_IsArray(temps_arr))
    {
        LOG_WARN("missing \"temps\" array in json");
        cJSON_Delete(root);
        return -1;
    }

    int arr_size = cJSON_GetArraySize(temps_arr);
    if (arr_size <= 0)
    {
        LOG_WARN("empty temps array");
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

    cJSON_Delete(root);

    app_weather_set_forecast(temps, arr_size);
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
