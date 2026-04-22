#include "lv_disp_port.h"
#include "bsp_lcd.h"

#define LV_PORT_BUF_SIZE (BSP_LCD_HEIGHT * BSP_LCD_WIDTH / 10 * sizeof(lv_color_t))

static bool _lv_port_trans_done_cb(void *ctx);
static void _lv_port_flush_cb(lv_display_t *drv, const lv_area_t *area, uint8_t *color_map);

static const char *TAG = "lv_disp_port.c";

static lv_display_t *disp_drv;

void lv_port_disp_init(void)
{
    lv_color_t *buf1 = heap_caps_malloc(LV_PORT_BUF_SIZE, MALLOC_CAP_DMA);
    assert(buf1);
    lv_color_t *buf2 = heap_caps_malloc(LV_PORT_BUF_SIZE, MALLOC_CAP_DMA);
    assert(buf2);

    disp_drv = lv_display_create(BSP_LCD_WIDTH, BSP_LCD_HEIGHT);
    assert(disp_drv);

    lv_display_set_buffers(disp_drv, buf1, buf2, LV_PORT_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp_drv, _lv_port_flush_cb);

    bsp_lcd_init(_lv_port_trans_done_cb, disp_drv);

    ESP_LOGI(TAG, "lvgl port display driver init");
}

static bool _lv_port_trans_done_cb(void *ctx)
{
    lv_display_t *drv = (lv_display_t *)ctx;
    lv_display_flush_ready(drv);
    return false;
}

static void _lv_port_flush_cb(lv_display_t *drv, const lv_area_t *area, uint8_t *color_map)
{
    bsp_lcd_disp_flush(area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_map);
}
