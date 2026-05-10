#include "app_ui.h"
#include "ui.h"


static void _app_ui_task(void *arg);

TaskHandle_t g_ui_handle;

void app_ui_init(void)
{
    xTaskCreate(_app_ui_task, "app gui", 10240, NULL, 5, &g_ui_handle);
}

static void _app_ui_task(void *arg)
{
    ui_init();
    while (1)
    {
        uint32_t delay = lv_task_handler();
        vTaskDelay(delay >= 5 ? delay : 5);
    }
}
