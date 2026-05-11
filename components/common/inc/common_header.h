#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <math.h>

#include "ek_def.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "freertos/stream_buffer.h"
#include "freertos/timers.h"

#include "esp_log.h"
#include "esp_err.h"

#define FILE_TAG(x)           static const char *__LOG_TAG__ = x

#define LOG_DEBUG(fmt, ...)   ESP_LOGD(__LOG_TAG__, "[L:%d F:%s]:" fmt, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)    ESP_LOGI(__LOG_TAG__, "[L:%d F:%s]:" fmt, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)    ESP_LOGW(__LOG_TAG__, "[L:%d F:%s]:" fmt, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)   ESP_LOGE(__LOG_TAG__, "[L:%d F:%s]:" fmt, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_VERBOSE(fmt, ...) ESP_LOGV(__LOG_TAG__, "[L:%d F:%s]:" fmt, __LINE__, __func__, ##__VA_ARGS__)
#define TODO(fmt, ...)        LOG_WARN("TODO: " fmt, ##__VA_ARGS__)

FORCE_INLINE_ATTR const char *unified_strerror(int err_code)
{
    if (err_code == ESP_OK)
    {
        return "ESP_OK (Success)";
    }
    if (err_code == ESP_FAIL)
    {
        return "ESP_FAIL (Generic Error)";
    }

    if (err_code > 0 && err_code < 256)
    {
        return strerror(err_code);
    }

    return esp_err_to_name((esp_err_t)err_code);
}

#ifdef __cplusplus
}
#endif

#endif // COMMON_HEADER_H
