#include "app_ui.h"
#include "ui.h"
#include "lv_demos.h"

static void _app_ui_task(void *arg);

TaskHandle_t g_ui_handle;

void app_ui_init(void)
{
    // ui_init();
    lv_demo_benchmark();
    xTaskCreatePinnedToCore(_app_ui_task, "app gui", 10240, NULL, 5, &g_ui_handle, 1);
}

static void _app_ui_task(void *arg)
{
    while (1)
    {
        lv_lock();
        ui_tick();
        lv_unlock();

        // lv_task_handler 内部会自己调用 lv_lock/lv_unlock
        uint32_t delay = lv_task_handler();
        TickType_t sleep_ms = delay >= 5 ? pdMS_TO_TICKS(delay) : pdMS_TO_TICKS(5);
        vTaskDelay(sleep_ms);
    }
}
