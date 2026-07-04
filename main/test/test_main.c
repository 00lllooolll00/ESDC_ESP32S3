#include "common_header.h"
#include "test_main.h"
#include "test_weather.h"

EK_LOG_FILE_TAG("test_main");

void test_run_all(void)
{
    EK_LOG_INFO("=== running all tests ===");
    test_weather_run();
    // 未来新增测试在此追加：test_xxx_run();
    EK_LOG_INFO("=== all tests done ===");
}
