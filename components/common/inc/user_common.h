#ifndef USER_COMMON_H
#define USER_COMMON_H

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

#define FILE_TAG(x)      static const char *__LOG_TAG__ = x

#define LOG_DBUG(...)    ESP_LOGD(__LOG_TAG__, __VA_ARGS__)
#define LOG_INFO(...)    ESP_LOGI(__LOG_TAG__, __VA_ARGS__)
#define LOG_WARN(...)    ESP_LOGW(__LOG_TAG__, __VA_ARGS__)
#define LOG_ERROR(...)   ESP_LOGE(__LOG_TAG__, __VA_ARGS__)
#define LOG_VERBOSE(...) ESP_LOGV(__LOG_TAG__, __VA_ARGS__)

#endif // USER_COMMON_H
