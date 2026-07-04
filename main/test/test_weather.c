#include "common_header.h"
#include "app_weather.h"

EK_LOG_FILE_TAG("test_weather");

static const char *s_test_json = "{\"current_temp\":25.5,\"max_temp\":32,\"min_temp\":18,"
                                 "\"humidity\":65,\"wind_speed\":12,\"type\":\"sunny\","
                                 "\"temps\":[18,19,20,22,24,26,28,30,32,31,29,27,25,23,22,21,20,19,18,19,21,23,24,25]}";

void test_weather_run(void)
{
    EK_LOG_INFO("test: injecting weather data");
    app_weather_inject_json(s_test_json);
}
