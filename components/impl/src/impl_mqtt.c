#include "impl_mqtt.h"

#include "mqtt_client.h"
#include "esp_event.h"

FILE_TAG("impl_mqtt");

static plat_mqtt_dev_t *s_mqtt_dev;

static int _mqtt_dev_init(void);
static int _mqtt_dev_deinit(void);
static int _mqtt_dev_suspend(void);
static int _mqtt_dev_resume(void);

static const plat_dev_ops_t s_mqtt_base_ops = {
    .init    = _mqtt_dev_init,
    .deinit  = _mqtt_dev_deinit,
    .suspend = _mqtt_dev_suspend,
    .resume  = _mqtt_dev_resume,
};

static int _mqtt_start(void);
static int _mqtt_stop(void);
static int _mqtt_reconnect(void);
static int _mqtt_disconnect(void);
static int _mqtt_publish(const char *topic, const char *data, int len, int qos);
static int _mqtt_subscribe(const char *topic, int qos);
static int _mqtt_unsubscribe(const char *topic);

static const plat_mqtt_ops_t s_mqtt_ops = {
    .start       = _mqtt_start,
    .stop        = _mqtt_stop,
    .reconnect   = _mqtt_reconnect,
    .disconnect  = _mqtt_disconnect,
    .publish     = _mqtt_publish,
    .subscribe   = _mqtt_subscribe,
    .unsubscribe = _mqtt_unsubscribe,
};

static void _mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

int impl_mqtt_register(plat_mqtt_dev_t *mqtt_dev)
{
    plat_mqtt_dev_register(mqtt_dev, "mqtt", &s_mqtt_base_ops, &s_mqtt_ops, NULL, NULL);
    s_mqtt_dev = mqtt_dev;
    return 0;
}

static int _mqtt_dev_init(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = s_mqtt_dev->broker_url,
    };
    s_mqtt_dev->client = esp_mqtt_client_init(&mqtt_cfg);
    if (!s_mqtt_dev->client) return -1;

    ESP_ERROR_CHECK(esp_mqtt_client_register_event(s_mqtt_dev->client, ESP_EVENT_ANY_ID, _mqtt_event_handler, NULL));
    return 0;
}

static int _mqtt_dev_deinit(void)
{
    if (s_mqtt_dev->client)
    {
        esp_mqtt_client_destroy(s_mqtt_dev->client);
        s_mqtt_dev->client = NULL;
    }
    return 0;
}

static int _mqtt_dev_suspend(void)
{
    return 0;
}

static int _mqtt_dev_resume(void)
{
    return 0;
}

static int _mqtt_start(void)
{
    return esp_mqtt_client_start(s_mqtt_dev->client);
}

static int _mqtt_stop(void)
{
    return esp_mqtt_client_stop(s_mqtt_dev->client);
}

static int _mqtt_reconnect(void)
{
    return esp_mqtt_client_reconnect(s_mqtt_dev->client);
}

static int _mqtt_disconnect(void)
{
    return esp_mqtt_client_disconnect(s_mqtt_dev->client);
}

static int _mqtt_publish(const char *topic, const char *data, int len, int qos)
{
    return esp_mqtt_client_enqueue(s_mqtt_dev->client, topic, data, len, qos, 0, false);
}

static int _mqtt_subscribe(const char *topic, int qos)
{
    return esp_mqtt_client_subscribe_single(s_mqtt_dev->client, topic, qos);
}

static int _mqtt_unsubscribe(const char *topic)
{
    return esp_mqtt_client_unsubscribe(s_mqtt_dev->client, topic);
}

#define MQTT_LOG_CHECK_ERROR(err_code, ...) \
    if (err_code) LOG_ERROR(__VA_ARGS__)

static void _mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;

    switch ((esp_mqtt_event_id_t)event_id)
    {
        case MQTT_EVENT_CONNECTED:
            LOG_INFO("mqtt connected");
            if (s_mqtt_dev && s_mqtt_dev->cb[PLAT_MQTT_CB_CONNECTED])
                s_mqtt_dev->cb[PLAT_MQTT_CB_CONNECTED]();
            break;

        case MQTT_EVENT_DISCONNECTED:
            LOG_INFO("mqtt disconnected");
            if (s_mqtt_dev && s_mqtt_dev->cb[PLAT_MQTT_CB_DISCONNECTED])
                s_mqtt_dev->cb[PLAT_MQTT_CB_DISCONNECTED]();
            break;

        case MQTT_EVENT_SUBSCRIBED:
            LOG_INFO("mqtt subscribed, msg_id=%d", event->msg_id);
            if (s_mqtt_dev && s_mqtt_dev->cb[PLAT_MQTT_CB_SUBSCRIBE])
                s_mqtt_dev->cb[PLAT_MQTT_CB_SUBSCRIBE]();
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            LOG_INFO("mqtt unsubscribed, msg_id=%d", event->msg_id);
            if (s_mqtt_dev && s_mqtt_dev->cb[PLAT_MQTT_CB_UNSUBSCRIBE])
                s_mqtt_dev->cb[PLAT_MQTT_CB_UNSUBSCRIBE]();
            break;

        case MQTT_EVENT_PUBLISHED:
            LOG_INFO("mqtt published, msg_id=%d", event->msg_id);
            if (s_mqtt_dev && s_mqtt_dev->cb[PLAT_MQTT_CB_PUBLISH])
                s_mqtt_dev->cb[PLAT_MQTT_CB_PUBLISH]();
            break;

        case MQTT_EVENT_DATA:
            LOG_INFO("mqtt data: topic=%.*s", event->topic_len, event->topic);
            if (s_mqtt_dev && s_mqtt_dev->data_cb)
                s_mqtt_dev->data_cb(event->topic, event->topic_len, event->data, event->data_len);
            break;

        case MQTT_EVENT_ERROR:
            LOG_INFO("mqtt error");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
            {
                MQTT_LOG_CHECK_ERROR(event->error_handle->esp_tls_last_esp_err, "reported from esp-tls");
                MQTT_LOG_CHECK_ERROR(event->error_handle->esp_tls_stack_err, "reported from tls stack");
                MQTT_LOG_CHECK_ERROR(event->error_handle->esp_transport_sock_errno,
                                     "captured as transport's socket errno");
                LOG_INFO("last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;

        default:
            LOG_DEBUG("mqtt other event id:%d", event->event_id);
            break;
    }
}