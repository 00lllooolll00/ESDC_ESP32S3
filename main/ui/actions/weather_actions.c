#include "common_header.h"
#include "ui.h"
#include "app_weather.h"
#include "lvgl.h"

EK_LOG_FILE_TAG("weather_actions");

static lv_chart_series_t *s_temp_series;
static lv_obj_t *s_chart;

// UI 回调：预报数据更新时刷新折线图
static void _weather_ui_cb(const app_weather_forecast_t *fc, void *arg)
{
    (void)arg;
    if (!s_chart || !s_temp_series || fc->count <= 0)
    {
        return;
    }

    // int16_t 转 int32_t（lv_chart_set_series_values 要求 const int32_t*）
    int32_t buf[APP_WEATHER_MAX_POINTS];
    for (int i = 0; i < fc->count; i++)
    {
        buf[i] = fc->temps[i];
    }

    lv_chart_set_series_values(s_chart, s_temp_series, buf, fc->count);
    lv_chart_refresh(s_chart);
}

void weather_ui_init(void)
{
    // 在 weather 屏幕上动态创建折线图
    s_chart = lv_chart_create(weather);
    if (!s_chart)
    {
        EK_LOG_ERROR("failed to create weather chart");
        return;
    }

    // 位置和尺寸
    lv_obj_set_pos(s_chart, 50, 60);
    lv_obj_set_size(s_chart, 700, 300);

    // 配置折线图
    lv_chart_set_type(s_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(s_chart, APP_WEATHER_MAX_POINTS);
    // Y 轴范围：-10.0°C..50.0°C（×10）
    lv_chart_set_axis_range(s_chart, LV_CHART_AXIS_PRIMARY_Y, -100, 500);

    // 添加温度数据系列（红色折线）
    s_temp_series = lv_chart_add_series(s_chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    if (!s_temp_series)
    {
        EK_LOG_ERROR("failed to add chart series");
        return;
    }

    // 注册天气数据回调
    app_weather_register_ui_cb(_weather_ui_cb, NULL);

    EK_LOG_INFO("weather chart initialized");
}
