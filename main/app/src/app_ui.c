#include "app_ui.h"
#include "ek_export.h"
#include "ui.h"

EK_LOG_FILE_TAG("app_ui.c");

// weather 折线图初始化（定义在 main/ui/actions/weather_actions.c）
extern void weather_ui_init(void);
extern void wifi_actions_init(void);
extern void main_page_actions_init(void);

static void _app_ui_task(void *arg);

TaskHandle_t g_ui_handle;

void app_ui_init(void)
{
    EK_LOG_INFO("ek_export: APP app_ui_init");
    ui_init("S:/");
    weather_ui_init();
    wifi_actions_init();
    main_page_actions_init();
    xTaskCreatePinnedToCore(_app_ui_task, "app gui", 10240, NULL, 5, &g_ui_handle, 1);
}

// UI 初始化：加载 screens、起 gui task；依赖 LVGL disp/touch port 完成
EK_EXPORT_APP(app_ui_init, 4);

static void _app_ui_task(void *arg)
{
    while (1)
    {
        // lv_task_handler 内部会自己调用 lv_lock/lv_unlock
        uint32_t delay = lv_task_handler();
        TickType_t sleep_ms = delay >= 5 ? pdMS_TO_TICKS(delay) : pdMS_TO_TICKS(5);
        vTaskDelay(sleep_ms);
    }
}
