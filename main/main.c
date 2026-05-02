#include "nvs_flash.h"
#include "esp_timer.h"

#include "app.h"

#include "lvgl.h"
#include "lv_disp_port.h"

FILE_TAG("main.c");

static uint32_t _lv_port_tick_get_cb(void);

plat_lcd_dev_t g_lcd_dev;
plat_key_dev_t g_key_dev;
plat_led_dev_t g_led_dev;

static TaskHandle_t s_led_handle;
static TaskHandle_t s_key_handle;

void app_main(void)
{
    portDISABLE_INTERRUPTS();
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        LOG_ERROR("fail to init nvs flash:%s", esp_err_to_name(err));
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
        LOG_INFO("successfully reinit nvs flash");
    }
    portENABLE_INTERRUPTS();

    lv_init();
    lv_tick_set_cb(_lv_port_tick_get_cb);

    impl_exio_init();
    impl_exio_int_enable();

    impl_led_register(&g_led_dev);
    impl_key_register(&g_key_dev);
    impl_lcd_register(&g_lcd_dev);

    plat_led_dev_init(&g_led_dev);
    plat_key_dev_init(&g_key_dev);

    lv_port_disp_init(&g_lcd_dev);

    xTaskCreate(app_led_task, "app led", 1024, NULL, 1, &s_led_handle);
    xTaskCreate(app_key_task, "app key", 2048, NULL, 2, &s_key_handle);
    xTaskCreate(app_gui_task, "app gui", 10240, NULL, 5, NULL);
}

static uint32_t _lv_port_tick_get_cb(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}
