#ifndef APP_WEATHER_API_H
#define APP_WEATHER_API_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"
#include "app_weather.h"

// 定位信息（provider 按需选用 city 或 lat/lon）
typedef struct
{
    float lat;
    float lon;
    char city[64];
} weather_location_t;

// 天气数据 provider 接口：换平台只需新写一个实现并注册
typedef struct
{
    const char *name;
    // 拉取天气数据，填充 out 结构体（temps/count/current_temp/type/humidity/wind_speed）
    // 返回 true 成功，false 失败
    bool (*fetch)(const weather_location_t *loc, app_weather_forecast_t *out);
} weather_provider_t;

// 初始化：注册默认 provider + 启动定时拉取 task（EK_EXPORT_APP 自动调用）
void app_weather_api_init(void);

// 非阻塞手动触发一次拉取（通知 task）
void app_weather_api_request(void);

// 切换 provider（运行时换平台）
void app_weather_api_set_provider(const weather_provider_t *provider);

#ifdef __cplusplus
}
#endif

#endif // APP_WEATHER_API_H
