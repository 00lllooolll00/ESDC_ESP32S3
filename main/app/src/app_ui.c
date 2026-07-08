#include "app_ui.h"
#include "ek_export.h"
#include "ui.h"
#include "esp_heap_caps.h"

EK_LOG_FILE_TAG("app_ui.c");

// weather 折线图初始化（定义在 main/ui/actions/weather_actions.c）
extern void weather_ui_init(void);
extern void wifi_actions_init(void);
extern void main_page_actions_init(void);
extern void smart_home_actions_init(void);
extern void ai_chat_actions_init(void);

static void _app_ui_task(void *arg);

TaskHandle_t g_ui_handle;

void app_ui_init(void)
{
    EK_LOG_INFO("ek_export: APP app_ui_init");
    ui_init("S:/");
    weather_ui_init();
    wifi_actions_init();
    main_page_actions_init();
    smart_home_actions_init();
    ai_chat_actions_init();
    EK_LOG_INFO("after ui init: internal free=%u, total free=%u",
                (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                (unsigned)esp_get_free_heap_size());
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
