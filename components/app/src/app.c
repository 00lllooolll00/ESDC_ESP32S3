#include "app.h"
#include "bsp_led.h"
#include "bsp_exio.h"
#include "lv_disp_port.h"

static void app_led_task(void *arg);
static void app_gui_task(void *arg);

TaskHandle_t g_led_handle;

void app_init(void)
{
    xTaskCreate(app_led_task, "app led", 256, NULL, 1, &g_led_handle);
    xTaskCreate(app_gui_task, "gui_task", 4096 * 2, NULL, 5, NULL);
}

static void app_led_task(void *arg)
{
    while (1)
    {
        bsp_led_toggle();
        vTaskDelay(500);
    }
}

static void app_gui_task(void *arg)
{
    while (1)
    {
        uint32_t delay = lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(delay > 0 ? delay : 1));
    }
}
