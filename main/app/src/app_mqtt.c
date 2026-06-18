#include "app_mqtt.h"
#include "app_weather.h"
#include "impl_mqtt.h"
#include <string.h>

FILE_TAG("app_mqtt");

extern plat_mqtt_dev_t g_mqtt_dev;

static void _mqtt_data_cb(const char *topic, int topic_len, const char *data, int data_len);
static void _mqtt_connected_cb(void);

void app_mqtt_init(void)
{
    // 注册 MQTT 设备到平台层
    impl_mqtt_register(&g_mqtt_dev);

    // 配置 broker URL（占位值，broker 确定后改宏）
    g_mqtt_dev.broker_url = APP_MQTT_BROKER_URL;

    // 注册数据回调：收到消息时解析 JSON 注入天气数据
    plat_mqtt_dev_data_cb_register(&g_mqtt_dev, _mqtt_data_cb);

    // 注册连接回调：连上后自动订阅天气 topic
    plat_mqtt_dev_cb_register(&g_mqtt_dev, PLAT_MQTT_CB_CONNECTED, _mqtt_connected_cb);

    // 初始化并启动（broker 是占位 URL，连接会失败，只 LOG 不崩）
    int err = plat_mqtt_dev_init(&g_mqtt_dev);
    if (err != 0)
    {
        LOG_WARN("mqtt dev init failed: %s", unified_strerror(err));
        return;
    }

    err = plat_mqtt_dev_start(&g_mqtt_dev);
    if (err != 0)
    {
        LOG_WARN("mqtt start failed (broker=%s): %s", APP_MQTT_BROKER_URL, unified_strerror(err));
        return;
    }

    LOG_INFO("mqtt started, broker=%s", APP_MQTT_BROKER_URL);
}

static void _mqtt_connected_cb(void)
{
    LOG_INFO("mqtt connected, subscribing %s", APP_MQTT_WEATHER_TOPIC);
    int err = plat_mqtt_dev_subscribe(&g_mqtt_dev, APP_MQTT_WEATHER_TOPIC, 1);
    if (err < 0)
    {
        LOG_WARN("mqtt subscribe failed: %s", unified_strerror(err));
    }
}

static void _mqtt_data_cb(const char *topic, int topic_len, const char *data, int data_len)
{
    // data 非零终止，拷贝到本地 buffer 补 \0
    if (data_len <= 0 || data_len >= 512)
    {
        LOG_WARN("mqtt data length invalid: %d", data_len);
        return;
    }

    char buf[512];
    memcpy(buf, data, data_len);
    buf[data_len] = '\0';

    LOG_INFO("mqtt data on %.*s: %s", topic_len, topic, buf);

    // 解析 JSON 并注入天气数据
    app_weather_inject_json(buf);
}
