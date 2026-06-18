#include "lv_disp_port.h"
#include "impl_rgblcd.h"
#include "esp_timer.h"
#include "ek_export.h"

FILE_TAG("lv_disp_prot.c");

static bool _lv_port_trans_done_cb(void *ctx);
static void _lv_port_flush_cb(lv_display_t *drv, const lv_area_t *area, uint8_t *color_map);

static plat_lcd_dev_t *s_lcd_dev;
static SemaphoreHandle_t s_trans_sem;

static uint32_t _lv_port_tick_get_cb(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}

// LVGL 核心 + tick 源：APP 层最早执行，disp/touch port 的前提
static void _auto_lv_core_init(void)
{
    LOG_INFO("ek_export: APP _auto_lv_core_init (lv_init + tick)");
    lv_init();
    lv_tick_set_cb(_lv_port_tick_get_cb);
}

EK_EXPORT_APP(_auto_lv_core_init, 0);

// 显示端口初始化：依赖 lv_init 与 rgblcd register，用访问器取设备
void lv_port_disp_init(void)
{
    LOG_INFO("ek_export: APP lv_port_disp_init");
    plat_lcd_dev_t *lcd_dev = impl_rgblcd_dev();
    assert(lcd_dev);
    s_lcd_dev = lcd_dev;

    s_trans_sem = xSemaphoreCreateCounting(1, 0);
    assert(s_trans_sem);

    // 在 init 之前设置 flush 完成回调，impl 的 init 会读取并传给 bsp
    lcd_dev->flush_done_cb = _lv_port_trans_done_cb;
    lcd_dev->flush_done_cb_arg = NULL;

    lv_display_t *disp_drv = lv_display_create(lcd_dev->width, lcd_dev->height);
    assert(disp_drv);
    lv_display_set_color_format(disp_drv, LV_COLOR_FORMAT_RGB565);

    // 设置回调
    lcd_dev->flush_done_cb_arg = disp_drv;
    plat_lcd_dev_init(lcd_dev);

    void *fb1 = plat_lcd_dev_get_fb(lcd_dev, 0);
    void *fb2 = plat_lcd_dev_get_fb(lcd_dev, 1);
    assert(fb1 && fb2);

    lv_display_set_buffers(
        disp_drv, fb1, fb2, s_lcd_dev->height * s_lcd_dev->width * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_DIRECT);

    lv_display_set_flush_cb(disp_drv, _lv_port_flush_cb);

    LOG_INFO("lvgl port display driver init ok");
}

EK_EXPORT_APP(lv_port_disp_init, 1);

static bool _lv_port_trans_done_cb(void *ctx)
{
    BaseType_t need_yield = pdFALSE;
    xSemaphoreGiveFromISR(s_trans_sem, &need_yield);
    return (need_yield == pdTRUE);
}

static void _lv_port_flush_cb(lv_display_t *drv, const lv_area_t *area, uint8_t *color_map)
{
    if (lv_display_flush_is_last(drv))
    {
        plat_lcd_dev_flush(s_lcd_dev, 0, 0, s_lcd_dev->width + 1, s_lcd_dev->height + 1, color_map);
        xSemaphoreTake(s_trans_sem, 0);
        xSemaphoreTake(s_trans_sem, portMAX_DELAY);
        lv_display_flush_ready(drv);
    }
    else
    {
        lv_display_flush_ready(drv);
    }
}
