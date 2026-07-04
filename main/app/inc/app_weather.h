#ifndef APP_WEATHER_H
#define APP_WEATHER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"

// 最多 24 个温度点（24 小时预报）
#define APP_WEATHER_MAX_POINTS 24

typedef enum
{
    APP_WEATHER_TYPE_UNKNOWN = 0,
    APP_WEATHER_TYPE_SUNNY,
    APP_WEATHER_TYPE_CLOUDY,
    APP_WEATHER_TYPE_OVERCAST,
    APP_WEATHER_TYPE_RAIN,
    APP_WEATHER_TYPE_THUNDER,
    APP_WEATHER_TYPE_SNOW,
} app_weather_type_t;

/**
 * @brief 温度预报数据
 *
 * temps 为实际温度 ×10（如 255 表示 25.5°C），避免浮点
 */
typedef struct
{
    int16_t temps[APP_WEATHER_MAX_POINTS];
    int count;
    int16_t current_temp;
    int16_t max_temp;
    int16_t min_temp;
    uint8_t humidity;
    uint8_t wind_speed;
    app_weather_type_t type;
} app_weather_forecast_t;

/**
 * @brief UI 回调类型：数据更新时在 lv_lock 保护下调用
 */
typedef void (*app_weather_ui_cb_t)(const app_weather_forecast_t *fc, void *arg);

/**
 * @brief 初始化天气数据层（创建互斥锁）
 */
void app_weather_init(void);

/**
 * @brief 核心注入 API：设置一组温度预报
 *
 * @param temps 温度数组，值为实际温度 ×10（如 255 = 25.5°C）
 * @param count 温度点数；超过 APP_WEATHER_MAX_POINTS 时截断
 *
 * 线程安全：内部互斥锁保护存储；触发 UI 回调时用 lv_lock/lv_unlock 包裹
 */
void app_weather_set_forecast(const int16_t *temps, int count);

/**
 * @brief 设置当前天气状态（类型/当前温度/湿度/风速）
 *
 * @param type 天气类型枚举
 * @param current_temp 当前温度 ×10（如 255 = 25.5°C）
 * @param humidity 湿度百分比
 * @param wind_speed 风速 km/h
 */
void app_weather_set_current(app_weather_type_t type, int16_t current_temp, uint8_t humidity, uint8_t wind_speed);

/**
 * @brief 天气类型枚举转中文名称
 */
const char *app_weather_type_name(app_weather_type_t t);

/**
 * @brief 串口测试入口：解析 JSON 并注入预报数据
 *
 * @param json_str JSON 字符串，格式 {"current_temp":25.5,"type":"sunny","humidity":65,"wind_speed":12,"temps":[...]}（仅 temps 必填，其余可选；max/min 从 temps 派生）
 * @return 0 成功，-1 解析失败
 */
int app_weather_inject_json(const char *json_str);

/**
 * @brief 注册 UI 回调，数据更新时在 lv_lock 保护下调用
 *
 * @param cb 回调函数指针
 * @param arg 透传给回调的用户参数
 */
void app_weather_register_ui_cb(app_weather_ui_cb_t cb, void *arg);

/**
 * @brief 获取最新预报快照（线程安全拷贝）
 *
 * @return 预报数据的值拷贝
 */
app_weather_forecast_t app_weather_get_forecast(void);

#ifdef __cplusplus
}
#endif

#endif // APP_WEATHER_H
