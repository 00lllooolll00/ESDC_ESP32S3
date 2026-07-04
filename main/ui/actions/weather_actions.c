#include "common_header.h"
#include "ui.h"
#include "app_weather.h"
#include "app_ip_location.h"
#include "app_weather_api.h"
#include "lvgl.h"

EK_LOG_FILE_TAG("weather_actions");

static lv_chart_series_t *s_temp_series;
static lv_obj_t *s_chart;
static lv_obj_t *s_city_lbl;
static lv_obj_t *s_type_lbl;
static lv_obj_t *s_temp_lbl;
static lv_obj_t *s_max_lbl;
static lv_obj_t *s_min_lbl;
static lv_obj_t *s_hum_lbl;
static lv_obj_t *s_wind_lbl;

// 定点温度格式化为 "25°" 或 "25.5°"（负温 "-5°"）
static void _fmt_temp(char *buf, size_t len, int16_t temp_x10)
{
    int val = temp_x10;
    int abs_val = val < 0 ? -val : val;
    int int_part = abs_val / 10;
    int frac = abs_val % 10;
    if (frac == 0)
    {
        snprintf(buf, len, "%s%d°", val < 0 ? "-" : "", int_part);
    }
    else
    {
        snprintf(buf, len, "%s%d.%d°", val < 0 ? "-" : "", int_part, frac);
    }
}

// UI 回调：预报数据更新时刷新折线图与所有 label
static void _weather_ui_cb(const app_weather_forecast_t *fc, void *arg)
{
    (void)arg;
    if (!fc)
    {
        return;
    }

    // 折线图刷新
    if (s_chart && s_temp_series && fc->count > 0)
    {
        int32_t buf[APP_WEATHER_MAX_POINTS];
        for (int i = 0; i < fc->count; i++)
        {
            buf[i] = fc->temps[i];
        }
        lv_chart_set_series_values(s_chart, s_temp_series, buf, fc->count);
        lv_chart_refresh(s_chart);
    }

    // 当前温度
    if (s_temp_lbl)
    {
        char tbuf[16];
        _fmt_temp(tbuf, sizeof(tbuf), fc->current_temp);
        lv_label_set_text(s_temp_lbl, tbuf);
    }

    // 最高/最低温
    if (s_max_lbl)
    {
        char tbuf[16];
        _fmt_temp(tbuf, sizeof(tbuf), fc->max_temp);
        lv_label_set_text(s_max_lbl, tbuf);
    }
    if (s_min_lbl)
    {
        char tbuf[16];
        _fmt_temp(tbuf, sizeof(tbuf), fc->min_temp);
        lv_label_set_text(s_min_lbl, tbuf);
    }

    // 天气类型
    if (s_type_lbl)
    {
        lv_label_set_text(s_type_lbl, app_weather_type_name(fc->type));
    }

    // 湿度
    if (s_hum_lbl)
    {
        char tbuf[32];
        snprintf(tbuf, sizeof(tbuf), "%d%%", fc->humidity);
        lv_label_set_text(s_hum_lbl, tbuf);
    }

    // 风速
    if (s_wind_lbl)
    {
        char tbuf[32];
        snprintf(tbuf, sizeof(tbuf), "%dkm/h", fc->wind_speed);
        lv_label_set_text(s_wind_lbl, tbuf);
    }
}

// UI 回调：城市名更新时刷新 weather_city label
static void _city_ui_cb(const char *city, void *arg)
{
    (void)arg;
    if (s_city_lbl && city && city[0])
    {
        lv_label_set_text(s_city_lbl, city);
    }
}

void weather_ui_init(void)
{
    // 找折线图卡片容器，找不到回退到 weather 屏
    lv_obj_t *card = lv_obj_find_by_name(weather, "weather_chart_card");
    lv_obj_t *parent = card ? card : weather;

    // 在卡片内创建折线图
    s_chart = lv_chart_create(parent);
    if (!s_chart)
    {
        EK_LOG_ERROR("failed to create weather chart");
        return;
    }

    // 位置和尺寸（卡片内留 8px 边距）
    lv_obj_set_pos(s_chart, 8, 8);
    lv_obj_set_size(s_chart, lv_obj_get_width(parent) - 16, lv_obj_get_height(parent) - 16);

    // 不透明实色背景（和卡片同色，无 alpha 混合）
    lv_obj_set_style_bg_opa(s_chart, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(s_chart, lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_radius(s_chart, 0, 0);
    lv_obj_set_style_border_width(s_chart, 0, 0);
    lv_obj_set_style_pad_all(s_chart, 8, 0);

    // 配置折线图
    lv_chart_set_type(s_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(s_chart, APP_WEATHER_MAX_POINTS);
    // Y 轴范围：-10.0°C..50.0°C（×10）
    lv_chart_set_axis_range(s_chart, LV_CHART_AXIS_PRIMARY_Y, -100, 500);
    // 5 条水平网格线，0 条垂直
    lv_chart_set_div_line_count(s_chart, 5, 0);

    // 添加温度数据系列（暖橙色折线）
    s_temp_series = lv_chart_add_series(s_chart, lv_color_hex(0xff9f43), LV_CHART_AXIS_PRIMARY_Y);
    if (!s_temp_series)
    {
        EK_LOG_ERROR("failed to add chart series");
        return;
    }

    // 折线样式：加粗线 + 数据点 + 网格线色
    lv_obj_set_style_line_width(s_chart, 3, LV_PART_ITEMS);
    lv_obj_set_style_size(s_chart, 4, 4, LV_PART_INDICATOR);
    lv_obj_set_style_line_color(s_chart, lv_color_hex(0x3a5060), LV_PART_MAIN);

    // 缓存所有 label
    s_city_lbl = lv_obj_find_by_name(weather, "weather_city");
    s_type_lbl = lv_obj_find_by_name(weather, "weather_type");
    s_temp_lbl = lv_obj_find_by_name(weather, "weather_temp");
    s_max_lbl = lv_obj_find_by_name(weather, "weather_max_temp");
    s_min_lbl = lv_obj_find_by_name(weather, "weather_min_temp");
    s_hum_lbl = lv_obj_find_by_name(weather, "weather_humidity");
    s_wind_lbl = lv_obj_find_by_name(weather, "weather_wind");

    // 设大号字体（montserrat 内置字体，XML 不便引用）
    if (s_temp_lbl)
    {
        lv_obj_set_style_text_font(s_temp_lbl, &lv_font_montserrat_26, 0);
    }
    if (s_max_lbl)
    {
        lv_obj_set_style_text_font(s_max_lbl, &lv_font_montserrat_20, 0);
    }
    if (s_min_lbl)
    {
        lv_obj_set_style_text_font(s_min_lbl, &lv_font_montserrat_20, 0);
    }

    // 天气图标（可选装饰，找不到则跳过）
    lv_obj_t *icon = lv_obj_find_by_name(weather, "weather_icon");
    if (icon)
    {
        lv_image_set_src(icon, "S:/images/weather.bin");
        lv_obj_set_style_image_recolor(icon, lv_color_white(), 0);
        lv_obj_set_style_image_recolor_opa(icon, LV_OPA_COVER, 0);
    }

    // 注册天气数据回调
    app_weather_register_ui_cb(_weather_ui_cb, NULL);
    // 注册 IP 定位城市名回调
    app_ip_location_register_ui_cb(_city_ui_cb, NULL);

    // 立即用现有数据刷新一次
    app_weather_forecast_t fc = app_weather_get_forecast();
    _weather_ui_cb(&fc, NULL);

    char city_buf[64];
    app_ip_location_get_city(city_buf, sizeof(city_buf));
    _city_ui_cb(city_buf, NULL);

    EK_LOG_INFO("weather ui initialized");
}

// 刷新按钮回调：点击立即触发天气 API 拉取最新数据
void action_weather_refresh(lv_event_t *e)
{
    (void)e;
    EK_LOG_INFO("weather: manual refresh requested");
    app_weather_api_request();
}
