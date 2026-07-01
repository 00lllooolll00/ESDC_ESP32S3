#include "app_ui.h"
#include "ek_export.h"
#include "ui.h"

EK_LOG_FILE_TAG("app_ui.c");

// weather 折线图初始化（定义在 main/ui/actions/weather_actions.c）
extern void weather_ui_init(void);

static void _app_ui_task(void *arg);

TaskHandle_t g_ui_handle;
lv_font_t *g_ui_font_chinese_3500_14;

void app_ui_init(void)
{
    EK_LOG_INFO("ek_export: APP app_ui_init");
    g_ui_font_chinese_3500_14 = lv_binfont_create("S:/ui_font_chinese_3500_14.bin");
    assert(g_ui_font_chinese_3500_14);
    ui_init();
    weather_ui_init();
    xTaskCreatePinnedToCore(_app_ui_task, "app gui", 10240, NULL, 5, &g_ui_handle, 1);
}

// UI 初始化：加载字体/screens、起 gui task；依赖 LVGL disp/touch port 完成
EK_EXPORT_APP(app_ui_init, 4);
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
