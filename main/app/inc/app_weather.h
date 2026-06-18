#ifndef APP_WEATHER_H
#define APP_WEATHER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"

// 最多 24 个温度点（24 小时预报）
#define APP_WEATHER_MAX_POINTS 24

/**
 * @brief 温度预报数据
 *
 * temps 为实际温度 ×10（如 255 表示 25.5°C），避免浮点
 */
typedef struct
{
    int16_t temps[APP_WEATHER_MAX_POINTS];
    int count;
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
 * @brief 串口测试入口：解析 JSON 并注入预报数据
 *
 * @param json_str JSON 字符串，格式 {"temps":[25.5,26.0,...]}
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
