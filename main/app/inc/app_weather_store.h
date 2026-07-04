#ifndef APP_WEATHER_STORE_H
#define APP_WEATHER_STORE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"
#include "app_weather.h"

/**
 * @brief 初始化：从 NVS 读取缓存天气数据并注入 app_weather 模块
 *
 * 由 EK_EXPORT_APP 自动调用（level 3，在 app_weather_init level 2 之后）。
 * 此时 UI 回调未注册，数据静默写入 s_forecast，不触发 UI 刷新。
 * 失败（NVS 无数据/magic 不匹配）打 WARN 日志，不阻塞启动。
 */
void app_weather_store_init(void);

/**
 * @brief 持久化：把天气数据 + 当前时间戳写入 NVS
 *
 * 在 app_weather_api task fetch 成功后调用。
 * 失败打 WARN 日志，不影响天气功能。
 *
 * @param fc 天气预报数据快照
 */
void app_weather_store_save(const app_weather_forecast_t *fc);

#ifdef __cplusplus
}
#endif

#endif // APP_WEATHER_STORE_H
