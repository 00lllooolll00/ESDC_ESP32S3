#include "app.h"
#include "bsp_led.h"
#include "bsp_exio.h"
#include "lv_disp_port.h"

static void app_led_task(void *arg);
static void app_gui_task(void *arg);

TaskHandle_t g_led_handle;

void app_init(void)
{
    bsp_exio_init();
    bsp_led_init();

    lv_init();
    lv_port_disp_init();

    app_gui_init();

    xTaskCreate(app_led_task, "app led", 1024, NULL, 1, &g_led_handle);
    xTaskCreate(app_gui_task, "app gui", 10240, NULL, 5, NULL);
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
        uint32_t delay = lv_task_handler();
        vTaskDelay(delay >= 5 ? delay : 5);
    }
}
