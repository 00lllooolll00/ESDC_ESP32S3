#include "app.h"
#include "bsp_led.h"
#include "bsp_exio.h"
#include "bsp_key.h"
#include "lv_disp_port.h"

FILE_TAG("app.c");

static void app_led_task(void *arg);
static void app_gui_task(void *arg);
static void app_key_task(void *arg);

TaskHandle_t g_led_handle;
TaskHandle_t g_key_handle;

void app_init(void)
{
    bsp_exio_init();
    bsp_led_init();
    bsp_key_init();

    lv_init();
    lv_port_disp_init();

    app_gui_init();

    xTaskCreate(app_led_task, "app led", 1024, NULL, 1, &g_led_handle);
    xTaskCreate(app_key_task, "app key", 2048, NULL, 2, &g_key_handle);
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

static void app_key_task(void *arg)
{
    bsp_key_state_t key_old = BSP_KEY_STATE_NONE;
    TickType_t start_tick = xTaskGetTickCount();
    while (1)
    {
        bsp_key_state_t key_temp = bsp_key_read_raw();
        bsp_key_state_t key_val = key_temp & (key_old ^ key_temp);
        key_old = key_temp;

        if (key_val & BSP_KEY_STATE_BOOT) LOG_INFO("key down:%s", "boot");
        if (key_val & BSP_KEY_STATE_0) LOG_INFO("key down:%d", 0);
        if (key_val & BSP_KEY_STATE_1) LOG_INFO("key down:%d", 1);
        if (key_val & BSP_KEY_STATE_2) LOG_INFO("key down:%d", 2);
        if (key_val & BSP_KEY_STATE_3) LOG_INFO("key down:%d", 3);

        vTaskDelayUntil(&start_tick, 30);
    }
}
