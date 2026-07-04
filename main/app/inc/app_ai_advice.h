#ifndef APP_AI_ADVICE_H
#define APP_AI_ADVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"
#include "app_weather.h"

/**
 * @brief 用当前天气数据调 DeepSeek 生成 50 字建议，写入 out
 *
 * @param fc 天气预报数据快照
 * @param out 输出缓冲区
 * @param out_len 输出缓冲区大小
 * @return true 成功，false 失败（out 置空字符串）
 */
bool app_ai_advice_request(const app_weather_forecast_t *fc, char *out, size_t out_len);

#ifdef __cplusplus
}
#endif

#endif // APP_AI_ADVICE_H
