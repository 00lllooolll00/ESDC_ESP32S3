// esp-idf
#include "nvs_flash.h"
#include "esp_timer.h"
#include "esp_vfs_fat.h"
#include "esp_pm.h"

// app
#include "app.h"

// lvgl
#include "lvgl.h"
#include "lv_disp_port.h"
#include "lv_indev_port.h"

// impl
#include "impl_rgblcd.h"
#include "impl_touch.h"
#include "impl_wifi.h"
#include "impl_led.h"
#include "impl_key.h"
#include "impl_exio.h"
#include "impl_tts.h"
#include "impl_wifi.h"

FILE_TAG("main.c");

static uint32_t _lv_port_tick_get_cb(void);

plat_lcd_dev_t g_lcd_dev;
plat_key_dev_t g_key_dev;
plat_led_dev_t g_led_dev;
plat_touch_dev_t g_touch_dev;
plat_wifi_dev_t g_wifi_dev;
plat_tts_dev_t g_tts_dev;

void app_main(void)
{
    portDISABLE_INTERRUPTS();
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        LOG_ERROR("fail to init nvs flash:%s", unified_strerror(err));
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
        LOG_INFO("successfully reinit nvs flash");
    }
    portENABLE_INTERRUPTS();

    const esp_vfs_fat_mount_config_t vfs_mount_cfg = {
        .format_if_mount_failed = false,
        .max_files = 4,
        .allocation_unit_size = 4096,
    };
    esp_err_t vfs_err = esp_vfs_fat_spiflash_mount_ro("/vfs", "vfs", &vfs_mount_cfg);
    if (vfs_err != ESP_OK)
    {
        LOG_ERROR("fail to mount vfs: %s", unified_strerror(vfs_err));
    }
    else
    {
        LOG_INFO("vfs mounted at /vfs");
    }

    // 配置电源管理：启用 DFS 动态调频与自动 light sleep
    // 必须在持有 PM lock 的外设（RGB LCD 等）初始化之前调用，否则 PM 模块不会真正生效
    esp_pm_config_esp32s3_t pm_config = {
        .max_freq_mhz       = 240,
        .min_freq_mhz       = 80,
        .light_sleep_enable = true,
    };
    ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
    impl_exio_init();
    impl_led_register(&g_led_dev);
    impl_key_register(&g_key_dev);
    impl_rgblcd_register(&g_lcd_dev);
    impl_touch_register(&g_touch_dev, 5);
    impl_wifi_register(&g_wifi_dev);
    impl_tts_register(&g_tts_dev);
    plat_tts_dev_enable_amp(&g_tts_dev, 0); // 显式关闭功放（TTS 暂停时也静音，避免残留使能放大底噪）

    plat_led_dev_init(&g_led_dev);
    plat_key_dev_init(&g_key_dev);
    // touch 在 lv_port_touch_init 中初始化
    // rgblcd 在 lv_port_disp_init 中初始化

    lv_init();
    lv_tick_set_cb(_lv_port_tick_get_cb);
    lv_port_disp_init(&g_lcd_dev);
    lv_port_touch_init(&g_touch_dev, 5);

    app_wifi_init();
    app_led_init();
    app_key_intit();
    app_ui_init();
}

static uint32_t _lv_port_tick_get_cb(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}
