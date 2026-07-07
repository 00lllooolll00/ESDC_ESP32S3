#include "common_header.h"

#include "app_ip_location.h"
#include "ek_export.h"
#include "lvgl.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "cJSON.h"
#include <string.h>
#include <stdlib.h>
#include "app_weather_api.h"
#include "secrets.h"

#ifndef AMAP_API_KEY
#error "AMAP_API_KEY not defined. Copy main/secrets.h.example to main/secrets.h and fill in your key."
#endif

EK_LOG_FILE_TAG("app_ip_location");

#define APP_IP_LOCATION_URL        "https://restapi.amap.com/v3/ip?key=" AMAP_API_KEY
#define APP_IP_LOCATION_TIMEOUT_MS 8000
#define APP_IP_LOCATION_TASK_STACK 8192
#define APP_IP_LOCATION_TASK_PRIO  3
#define APP_IP_LOCATION_CITY_MAX   64
#define APP_IP_LOCATION_BUF_MAX    1024

static char s_city[APP_IP_LOCATION_CITY_MAX];
static float s_lat = 0;
static float s_lon = 0;
static SemaphoreHandle_t s_mutex;
static app_ip_location_ui_cb_t s_ui_cb;
static void *s_ui_cb_arg;
static TaskHandle_t s_task;

static bool _do_http_fetch(char *out, size_t out_len);
static void _ip_loc_task(void *arg);

void app_ip_location_init(void)
{
    EK_LOG_INFO("ek_export: APP app_ip_location_init");
    s_mutex = xSemaphoreCreateMutex();
    assert(s_mutex);
    s_city[0] = '\0';
    s_lat = 0;
    s_lon = 0;
    s_ui_cb = NULL;
    s_ui_cb_arg = NULL;
    s_task = NULL;

    BaseType_t ok =
        xTaskCreate(_ip_loc_task, "ip_loc", APP_IP_LOCATION_TASK_STACK, NULL, APP_IP_LOCATION_TASK_PRIO, &s_task);
    assert(ok == pdPASS);
}

EK_EXPORT_APP(app_ip_location_init, 2);

void app_ip_location_request(void)
{
    // 非阻塞通知拉取 task；可安全从持 lv_lock 的回调上下文调用
    if (s_task)
    {
        xTaskNotifyGive(s_task);
    }
}

void app_ip_location_set_city(const char *city)
{
    if (!s_mutex)
    {
        return;
    }

    xSemaphoreTake(s_mutex, portMAX_DELAY);
    if (city)
    {
        strncpy(s_city, city, APP_IP_LOCATION_CITY_MAX - 1);
        s_city[APP_IP_LOCATION_CITY_MAX - 1] = '\0';
    }
    else
    {
        s_city[0] = '\0';
    }
    xSemaphoreGive(s_mutex);

    EK_LOG_INFO("city updated: %s", s_city);

    // 触发 UI 回调，lv_lock 保护 LVGL 操作
    if (s_ui_cb)
    {
        lv_lock();
        s_ui_cb(s_city, s_ui_cb_arg);
        lv_unlock();
    }
}

void app_ip_location_get_city(char *buf, size_t len)
{
    if (!buf || len == 0)
    {
        return;
    }

    if (!s_mutex)
    {
        buf[0] = '\0';
        return;
    }

    xSemaphoreTake(s_mutex, portMAX_DELAY);
    strncpy(buf, s_city, len - 1);
    buf[len - 1] = '\0';
    xSemaphoreGive(s_mutex);
}

void app_ip_location_get_latlon(float *lat, float *lon)
{
    if (!lat || !lon)
    {
        return;
    }
    if (!s_mutex)
    {
        *lat = 0;
        *lon = 0;
        return;
    }
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    *lat = s_lat;
    *lon = s_lon;
    xSemaphoreGive(s_mutex);
}

void app_ip_location_register_ui_cb(app_ip_location_ui_cb_t cb, void *arg)
{
    if (!s_mutex)
    {
        return;
    }
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    s_ui_cb = cb;
    s_ui_cb_arg = arg;
    xSemaphoreGive(s_mutex);
}

// HTTP 拉取高德 IP 定位并解析 city；成功返回 true 且写入 out
static bool _do_http_fetch(char *out, size_t out_len)
{
    bool ok = false;
    char *buf = NULL;
    cJSON *root = NULL;

    esp_http_client_config_t cfg = {
        .url = APP_IP_LOCATION_URL,
        .method = HTTP_METHOD_GET,
        .timeout_ms = APP_IP_LOCATION_TIMEOUT_MS,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    esp_http_client_handle_t client = esp_http_client_init(&cfg);
    if (!client)
    {
        EK_LOG_WARN("http client init failed");
        return false;
    }

    // 用 open + fetch_headers 流式读取；不调 perform（perform 会消费并关闭响应，导致后续 read 读到空 body）
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK)
    {
        EK_LOG_WARN("http open failed: %s", unified_strerror(err));
        goto cleanup;
    }

    esp_http_client_fetch_headers(client);
    int status = esp_http_client_get_status_code(client);
    if (status != 200)
    {
        EK_LOG_WARN("http status %d", status);
        goto cleanup;
    }

    buf = malloc(APP_IP_LOCATION_BUF_MAX);
    if (!buf)
    {
        EK_LOG_WARN("buf alloc failed");
        goto cleanup;
    }

    // 循环读取 body（高德 body ~200B，通常一次读尽，循环保险）
    int total = 0;
    int n = 0;
    while (total < APP_IP_LOCATION_BUF_MAX - 1)
    {
        n = esp_http_client_read(client, buf + total, APP_IP_LOCATION_BUF_MAX - 1 - total);
        if (n < 0)
        {
            EK_LOG_WARN("http read error");
            goto cleanup;
        }
        if (n == 0)
        {
            break;
        }
        total += n;
    }
    buf[total] = '\0';

    if (total == 0)
    {
        EK_LOG_WARN("http empty body");
        goto cleanup;
    }

    root = cJSON_Parse(buf);
    if (!root)
    {
        EK_LOG_WARN("json parse failed: %s", buf);
        goto cleanup;
    }

    cJSON *status_obj = cJSON_GetObjectItem(root, "status");
    if (!status_obj || !cJSON_IsString(status_obj) || strcmp(status_obj->valuestring, "1") != 0)
    {
        EK_LOG_WARN("amap status not ok: %s",
                    (status_obj && cJSON_IsString(status_obj)) ? status_obj->valuestring : "(null)");
        goto cleanup;
    }

    cJSON *city_obj = cJSON_GetObjectItem(root, "city");
    if (!city_obj || !cJSON_IsString(city_obj) || city_obj->valuestring[0] == '\0')
    {
        city_obj = cJSON_GetObjectItem(root, "province");
    }
    if (!city_obj || !cJSON_IsString(city_obj) || city_obj->valuestring[0] == '\0')
    {
        EK_LOG_WARN("missing city field");
        goto cleanup;
    }

    // 解析经纬度（高德 rectangle 中心点）并持锁存储
    cJSON *rect_obj = cJSON_GetObjectItem(root, "rectangle");
    if (rect_obj && cJSON_IsString(rect_obj) && rect_obj->valuestring[0] != '\0')
    {
        float lon1, lat1, lon2, lat2;
        // 高德 rectangle 格式："经度,纬度;经度,纬度"（左下;右上）
        if (sscanf(rect_obj->valuestring, "%f,%f;%f,%f", &lon1, &lat1, &lon2, &lat2) == 4)
        {
            float parsed_lat = (lat1 + lat2) / 2.0f;
            float parsed_lon = (lon1 + lon2) / 2.0f;
            xSemaphoreTake(s_mutex, portMAX_DELAY);
            s_lat = parsed_lat;
            s_lon = parsed_lon;
            xSemaphoreGive(s_mutex);
            EK_LOG_INFO("lat=%.4f lon=%.4f", s_lat, s_lon);
        }
        else
        {
            EK_LOG_WARN("rectangle parse failed: %s", rect_obj->valuestring);
        }
    }

    strncpy(out, city_obj->valuestring, out_len - 1);
    out[out_len - 1] = '\0';
    ok = true;

cleanup:
    if (root)
    {
        cJSON_Delete(root);
    }
    if (buf)
    {
        free(buf);
    }
    esp_http_client_cleanup(client);
    return ok;
}

static void _ip_loc_task(void *arg)
{
    (void)arg;
    char city_out[APP_IP_LOCATION_CITY_MAX];

    while (1)
    {
        // 等待触发通知（WiFi 连接成功时 app_ip_location_request 发出）
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        city_out[0] = '\0';
        if (_do_http_fetch(city_out, sizeof(city_out)))
        {
            EK_LOG_INFO("got city: %s", city_out);
            app_ip_location_set_city(city_out);
            // 定位完成，通知天气 task 用新坐标拉取天气
            app_weather_api_request();
        }
    }
}
