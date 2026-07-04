#include "common_header.h"
#include "app_weather_api.h"
#include "app_weather.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "cJSON.h"
#include "miniz.h"
#include <stdlib.h>
#include <string.h>

EK_LOG_FILE_TAG("provider_qweather");

#define QWEATHER_API_KEY     "YOUR_QWEATHER_KEY"
#define QWEATHER_API_HOST    "p52r6yxrx2.re.qweatherapi.com"
#define QWEATHER_TIMEOUT_MS  30000
#define QWEATHER_NOW_BUF_MAX 4096
#define QWEATHER_24H_BUF_MAX 8192

// 中文天气描述 → 枚举（顺序敏感：雷>雪>雨>多云>阴>晴）
static app_weather_type_t _text_to_type(const char *text)
{
    if (!text)
    {
        return APP_WEATHER_TYPE_UNKNOWN;
    }
    if (strstr(text, "雷"))
    {
        return APP_WEATHER_TYPE_THUNDER;
    }
    if (strstr(text, "雪"))
    {
        return APP_WEATHER_TYPE_SNOW;
    }
    if (strstr(text, "雨"))
    {
        return APP_WEATHER_TYPE_RAIN;
    }
    if (strstr(text, "多云"))
    {
        return APP_WEATHER_TYPE_CLOUDY;
    }
    if (strstr(text, "阴"))
    {
        return APP_WEATHER_TYPE_OVERCAST;
    }
    if (strstr(text, "晴"))
    {
        return APP_WEATHER_TYPE_SUNNY;
    }
    return APP_WEATHER_TYPE_UNKNOWN;
}

static bool _gzip_payload_offset(const uint8_t *data, size_t len, size_t *offset)
{
    if (!data || len < 18 || !offset || data[0] != 0x1f || data[1] != 0x8b || data[2] != 8)
    {
        return false;
    }

    uint8_t flags = data[3];
    size_t pos = 10;

    if (flags & 0x04)
    {
        if (pos + 2 > len)
        {
            return false;
        }
        size_t extra_len = (size_t)data[pos] | ((size_t)data[pos + 1] << 8);
        pos += 2 + extra_len;
        if (pos > len)
        {
            return false;
        }
    }

    if (flags & 0x08)
    {
        while (pos < len && data[pos] != 0)
        {
            pos++;
        }
        pos++;
    }

    if (flags & 0x10)
    {
        while (pos < len && data[pos] != 0)
        {
            pos++;
        }
        pos++;
    }

    if (flags & 0x02)
    {
        pos += 2;
    }

    if (pos >= len - 8)
    {
        return false;
    }

    *offset = pos;
    return true;
}

static bool _decode_gzip_if_needed(const uint8_t *raw, size_t raw_len, char *out, size_t out_len)
{
    if (!raw || !out || out_len == 0)
    {
        return false;
    }

    if (raw_len >= 2 && raw[0] == 0x1f && raw[1] == 0x8b)
    {
        size_t payload_off = 0;
        if (!_gzip_payload_offset(raw, raw_len, &payload_off))
        {
            EK_LOG_WARN("gzip header invalid");
            return false;
        }

        size_t payload_len = raw_len - payload_off - 8;

        // 堆分配 decompressor（约 11KB），避免栈溢出
        tinfl_decompressor *decomp = calloc(1, sizeof(tinfl_decompressor));
        if (!decomp)
        {
            EK_LOG_WARN("gzip decompressor alloc failed");
            return false;
        }
        tinfl_init(decomp);

        size_t in_pos = 0;
        size_t out_pos = 0;
        int status = 0;

        do
        {
            size_t in_bytes = payload_len - in_pos;
            size_t out_bytes = out_len - 1 - out_pos;
            status = tinfl_decompress(decomp,
                                      (const mz_uint8 *)raw + payload_off + in_pos,
                                      &in_bytes,
                                      (mz_uint8 *)out,
                                      (mz_uint8 *)out + out_pos,
                                      &out_bytes,
                                      TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF);
            in_pos += in_bytes;
            out_pos += out_bytes;
        } while (status == TINFL_STATUS_NEEDS_MORE_INPUT || status == TINFL_STATUS_HAS_MORE_OUTPUT);

        free(decomp);

        if (status != TINFL_STATUS_DONE)
        {
            EK_LOG_WARN("gzip decompress failed, status=%d raw=%d payload=%d", status, (int)raw_len, (int)payload_len);
            return false;
        }

        out[out_pos] = '\0';
        return true;
    }

    size_t copy_len = raw_len < out_len - 1 ? raw_len : out_len - 1;
    memcpy(out, raw, copy_len);
    out[copy_len] = '\0';
    return copy_len > 0;
}

// HTTPS GET + X-QW-Api-Key header，body 写入 out
static bool _http_get(const char *url, char *out, size_t out_len)
{
    esp_http_client_config_t cfg = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .timeout_ms = QWEATHER_TIMEOUT_MS,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    esp_http_client_handle_t client = esp_http_client_init(&cfg);
    if (!client)
    {
        return false;
    }

    esp_http_client_set_header(client, "X-QW-Api-Key", QWEATHER_API_KEY);
    esp_http_client_set_header(client, "Accept-Encoding", "identity");

    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK)
    {
        EK_LOG_WARN("http open failed: %s", unified_strerror(err));
        esp_http_client_cleanup(client);
        return false;
    }

    esp_http_client_fetch_headers(client);
    int status = esp_http_client_get_status_code(client);
    if (status != 200)
    {
        EK_LOG_WARN("http status %d", status);
        esp_http_client_cleanup(client);
        return false;
    }

    uint8_t *raw = malloc(out_len);
    if (!raw)
    {
        EK_LOG_WARN("http raw buffer alloc failed");
        esp_http_client_cleanup(client);
        return false;
    }

    int total = 0;
    while (total < (int)out_len)
    {
        int n = esp_http_client_read(client, (char *)raw + total, out_len - total);
        if (n <= 0)
        {
            break;
        }
        total += n;
    }
    esp_http_client_cleanup(client);
    vTaskDelay(pdMS_TO_TICKS(300)); // 等 AES DMA buffer 释放，避免连续请求内存不足

    if (total == 0)
    {
        EK_LOG_WARN("http empty body");
        free(raw);
        return false;
    }

    bool ok = _decode_gzip_if_needed(raw, (size_t)total, out, out_len);
    free(raw);
    return ok;
}

static bool _qweather_fetch(const weather_location_t *loc, app_weather_forecast_t *out)
{
    char url[256];
    char *buf = NULL;
    cJSON *root = NULL;
    bool ok = false;

    // --- 实况 ---
    snprintf(
        url, sizeof(url), "https://%s/v7/weather/now?location=%.2f,%.2f&unit=m", QWEATHER_API_HOST, loc->lon, loc->lat);

    buf = malloc(QWEATHER_NOW_BUF_MAX);
    if (!buf)
    {
        return false;
    }

    if (!_http_get(url, buf, QWEATHER_NOW_BUF_MAX))
    {
        free(buf);
        return false;
    }

    root = cJSON_Parse(buf);
    if (!root)
    {
        EK_LOG_WARN("now json parse failed, total=%d, hex: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x "
                    "%02x %02x %02x %02x",
                    (int)strlen(buf),
                    (uint8_t)buf[0],
                    (uint8_t)buf[1],
                    (uint8_t)buf[2],
                    (uint8_t)buf[3],
                    (uint8_t)buf[4],
                    (uint8_t)buf[5],
                    (uint8_t)buf[6],
                    (uint8_t)buf[7],
                    (uint8_t)buf[8],
                    (uint8_t)buf[9],
                    (uint8_t)buf[10],
                    (uint8_t)buf[11],
                    (uint8_t)buf[12],
                    (uint8_t)buf[13],
                    (uint8_t)buf[14],
                    (uint8_t)buf[15]);
        EK_LOG_WARN("now json parse failed, body: %.200s", buf);
        free(buf);
        return false;
    }

    cJSON *now = cJSON_GetObjectItem(root, "now");
    if (now)
    {
        cJSON *t = cJSON_GetObjectItem(now, "temp");
        cJSON *tx = cJSON_GetObjectItem(now, "text");
        cJSON *h = cJSON_GetObjectItem(now, "humidity");
        cJSON *w = cJSON_GetObjectItem(now, "windSpeed");
        if (t && t->valuestring)
        {
            out->current_temp = (int16_t)(atoi(t->valuestring) * 10);
        }
        if (tx && tx->valuestring)
        {
            out->type = _text_to_type(tx->valuestring);
        }
        if (h && h->valuestring)
        {
            out->humidity = (uint8_t)atoi(h->valuestring);
        }
        if (w && w->valuestring)
        {
            out->wind_speed = (uint8_t)atoi(w->valuestring);
        }
    }
    cJSON_Delete(root);
    free(buf);
    buf = NULL;

    // --- 逐 24h ---
    snprintf(
        url, sizeof(url), "https://%s/v7/weather/24h?location=%.2f,%.2f&unit=m", QWEATHER_API_HOST, loc->lon, loc->lat);

    buf = malloc(QWEATHER_24H_BUF_MAX);
    if (!buf)
    {
        return false;
    }

    if (!_http_get(url, buf, QWEATHER_24H_BUF_MAX))
    {
        free(buf);
        // 实况已拿到，24h 失败不算完全失败
        return true;
    }

    root = cJSON_Parse(buf);
    if (!root)
    {
        EK_LOG_WARN("24h json parse failed");
        free(buf);
        return true;
    }

    cJSON *hourly = cJSON_GetObjectItem(root, "hourly");
    if (hourly && cJSON_IsArray(hourly))
    {
        int n = cJSON_GetArraySize(hourly);
        if (n > APP_WEATHER_MAX_POINTS)
        {
            n = APP_WEATHER_MAX_POINTS;
        }
        out->count = n;
        for (int i = 0; i < n; i++)
        {
            cJSON *item = cJSON_GetArrayItem(hourly, i);
            cJSON *t = cJSON_GetObjectItem(item, "temp");
            if (t && t->valuestring)
            {
                out->temps[i] = (int16_t)(atoi(t->valuestring) * 10);
            }
        }
    }
    cJSON_Delete(root);
    free(buf);


    // --- 3 天预报 ---
    snprintf(url, sizeof(url), "https://%s/v7/weather/3d?location=%.2f,%.2f", QWEATHER_API_HOST, loc->lon, loc->lat);
    buf = malloc(QWEATHER_NOW_BUF_MAX);
    if (buf && _http_get(url, buf, QWEATHER_NOW_BUF_MAX))
    {
        root = cJSON_Parse(buf);
        if (root)
        {
            cJSON *daily = cJSON_GetObjectItem(root, "daily");
            if (daily && cJSON_IsArray(daily))
            {
                int n = cJSON_GetArraySize(daily);
                if (n > APP_WEATHER_DAILY_MAX)
                {
                    n = APP_WEATHER_DAILY_MAX;
                }
                out->daily_count = n;
                for (int i = 0; i < n; i++)
                {
                    cJSON *item = cJSON_GetArrayItem(daily, i);
                    cJSON *tmax = cJSON_GetObjectItem(item, "tempMax");
                    cJSON *tmin = cJSON_GetObjectItem(item, "tempMin");
                    cJSON *txd = cJSON_GetObjectItem(item, "textDay");
                    if (tmax && tmax->valuestring)
                    {
                        out->dailies[i].temp_max = (int16_t)(atoi(tmax->valuestring) * 10);
                    }
                    if (tmin && tmin->valuestring)
                    {
                        out->dailies[i].temp_min = (int16_t)(atoi(tmin->valuestring) * 10);
                    }
                    if (txd && txd->valuestring)
                    {
                        strncpy(out->dailies[i].text_day, txd->valuestring, sizeof(out->dailies[i].text_day) - 1);
                    }
                }
                if (n > 0)
                {
                    out->max_temp = out->dailies[0].temp_max;
                    out->min_temp = out->dailies[0].temp_min;
                }
            }
            cJSON_Delete(root);
        }
    }
    free(buf);
    buf = NULL;

    ok = true;
    return ok;
}

const weather_provider_t g_qweather_provider = {
    .name = "QWeather",
    .fetch = _qweather_fetch,
};
