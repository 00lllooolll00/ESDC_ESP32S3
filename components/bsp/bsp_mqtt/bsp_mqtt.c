#include "bsp_mqtt.h"
#include "mqtt_client.h"
#include "esp_event.h"
#include "esp_netif.h"

FILE_TAG("bsp_mqtt.c");

#define MQTT_LOG_CHECK_ERROR(err_code, ...) \
    if (err_code) LOG_ERROR(__VA_ARGS__)

#define MQTT_DO_CALLBACK(func) \
    if (func) func()

static void _mqtt_event_hadnelr(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

static esp_mqtt_client_handle_t s_mqtt_client_hanler;
static void (*s_mqtt_connected_cb)(void);
static void (*s_mqtt_disconnected_cb)(void);
static void (*s_mqtt_publish_cb)(void);
static void (*s_mqtt_subsribe_cb)(void);
static void (*s_mqtt_unsubsribe_cb)(void);

void bsp_mqtt_init(const char *broker_base_url)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = broker_base_url,
    };
    s_mqtt_client_hanler = esp_mqtt_client_init(&mqtt_cfg);
    ESP_ERROR_CHECK(esp_mqtt_client_register_event(s_mqtt_client_hanler, ESP_EVENT_ANY_ID, _mqtt_event_hadnelr, NULL));
}

esp_err_t bsp_mqtt_start(void)
{
    return esp_mqtt_client_start(s_mqtt_client_hanler);
}

esp_err_t bsp_mqtt_stop(void)
{
    return esp_mqtt_client_stop(s_mqtt_client_hanler);
}

esp_err_t bsp_mqtt_reconnect(void)
{
    return esp_mqtt_client_reconnect(s_mqtt_client_hanler);
}

esp_err_t bsp_mqtt_disconnect(void)
{
    return esp_mqtt_client_disconnect(s_mqtt_client_hanler);
}

esp_err_t bsp_mqtt_publish(const char *topic, const char *data, int qos)
{
    return esp_mqtt_client_enqueue(s_mqtt_client_hanler, topic, data, strlen(data), qos, 0, false);
}

esp_err_t bsp_mqtt_subscribe(const char *topic, int qos)
{
    return esp_mqtt_client_subscribe_single(s_mqtt_client_hanler, topic, qos);
}

esp_err_t bsp_mqtt_unsubscribe(const char *topic)
{
    return esp_mqtt_client_unsubscribe(s_mqtt_client_hanler, topic);
}

void bsp_mqtt_register_cb(bsp_mqtt_cb_t type, void (*cb)(void))
{
    switch (type)
    {
        case BSP_MQTT_CB_CONNECTED:
            s_mqtt_connected_cb = cb;
            break;

        case BSP_MQTT_CB_DISCONNECTED:
            s_mqtt_disconnected_cb = cb;
            break;

        case BSP_MQTT_CB_PUBLISH:
            s_mqtt_publish_cb = cb;
            break;

        case BSP_MQTT_CB_SUBSCRIBE:
            s_mqtt_subsribe_cb = cb;
            break;

        case BSP_MQTT_CB_UNSUBSCRIBE:
            s_mqtt_unsubsribe_cb = cb;
            break;

        default:
            LOG_ERROR("unkwon type:%d", type);
    }
}

static void _mqtt_event_hadnelr(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    LOG_DEBUG("event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
        case MQTT_EVENT_CONNECTED:
            LOG_INFO("MQTT_EVENT_CONNECTED");
            MQTT_DO_CALLBACK(s_mqtt_connected_cb);
            break;

        case MQTT_EVENT_DISCONNECTED:
            LOG_INFO("MQTT_EVENT_DISCONNECTED");
            MQTT_DO_CALLBACK(s_mqtt_disconnected_cb);
            break;

        case MQTT_EVENT_SUBSCRIBED:
            LOG_INFO("MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            MQTT_DO_CALLBACK(s_mqtt_subsribe_cb);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            LOG_INFO("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            MQTT_DO_CALLBACK(s_mqtt_unsubsribe_cb);
            break;

        case MQTT_EVENT_PUBLISHED:
            LOG_INFO("MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            MQTT_DO_CALLBACK(s_mqtt_publish_cb);
            break;

        case MQTT_EVENT_DATA:
            LOG_INFO("MQTT_EVENT_DATA");
            LOG_INFO("TOPIC=%.*s", event->topic_len, event->topic);
            LOG_INFO("DATA=%.*s", event->data_len, event->data);
            break;

        case MQTT_EVENT_ERROR:
            LOG_INFO("MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
            {
                MQTT_LOG_CHECK_ERROR(event->error_handle->esp_tls_last_esp_err, "reported from esp-tls");
                MQTT_LOG_CHECK_ERROR(event->error_handle->esp_tls_stack_err, "reported from tls stack");
                MQTT_LOG_CHECK_ERROR(event->error_handle->esp_transport_sock_errno,
                                     "captured as transport's socket errno");
                LOG_INFO("Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;

        default:
            LOG_INFO("other event id:%d", event->event_id);
            break;
    }
}
