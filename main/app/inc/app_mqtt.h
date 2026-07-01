#ifndef APP_MQTT_H
#define APP_MQTT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"

// broker 未确定，此处为占位值
// broker 确定后改这两个宏即可接通真实数据
#define APP_MQTT_BROKER_URL    "mqtt://TODO.aliyuncs.com:1883"
#define APP_MQTT_WEATHER_TOPIC "/esdc/weather/forecast"

/**
 * @brief 初始化 MQTT 任务层
 *
 * 注册 plat_mqtt 设备、配置 broker、注册数据回调并启动连接
 * broker 为占位 URL 时连接会失败，只 EK_LOG_WARN 不崩溃
 */
void app_mqtt_init(void);

#ifdef __cplusplus
}
#endif

#endif // APP_MQTT_H
