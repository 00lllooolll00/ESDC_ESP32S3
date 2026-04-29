#include "app.h"
#include "impl_exio.h"
#include "lv_disp_port.h"
#include "impl_lcd.h"
#include "impl_key.h"
#include "impl_led.h"

FILE_TAG("app.c");

static void app_led_task(void *arg);
static void app_gui_task(void *arg);
static void app_key_task(void *arg);

static plat_lcd_dev_t s_lcd_dev;
static plat_key_dev_t s_key_dev;
static plat_led_dev_t s_led_dev;

TaskHandle_t g_led_handle;
TaskHandle_t g_key_handle;

void app_init(void)
{
    impl_exio_init();

    impl_led_register(&s_led_dev);
    impl_key_register(&s_key_dev);
    impl_lcd_register(&s_lcd_dev);

    plat_led_dev_init(&s_led_dev);
    plat_key_dev_init(&s_key_dev);

    lv_port_disp_init(&s_lcd_dev);

    app_gui_init();

    xTaskCreate(app_led_task, "app led", 1024, NULL, 1, &g_led_handle);
    xTaskCreate(app_key_task, "app key", 2048, NULL, 2, &g_key_handle);
    xTaskCreate(app_gui_task, "app gui", 10240, NULL, 5, NULL);
}

static void app_led_task(void *arg)
{
    while (1)
    {
        plat_led_dev_toggle(&s_led_dev);
        vTaskDelay(500);
    }
}

static void app_gui_task(void *arg)
{
    app_gui_init();
    while (1)
    {
        uint32_t delay = lv_task_handler();
        vTaskDelay(delay >= 5 ? delay : 5);
    }
}

static void app_key_task(void *arg)
{
    plat_key_state_t key_old = PLAT_KEY_STATE_NONE;
    TickType_t start_tick = xTaskGetTickCount();
    while (1)
    {
        plat_key_state_t key_temp;
        plat_key_dev_read_raw(&s_key_dev, &key_temp);
        plat_key_state_t key_val = key_temp & (key_old ^ key_temp);
        key_old = key_temp;

        if (key_val != PLAT_KEY_STATE_NONE)
        {
            LOG_INFO("key down:%x", key_val);
        }

        vTaskDelayUntil(&start_tick, 30);
    }
}
