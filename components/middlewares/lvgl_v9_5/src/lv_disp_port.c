#include "lv_disp_port.h"

FILE_TAG("lv_disp_prot.c");

static bool _lv_port_trans_done_cb(void *ctx);
static void _lv_port_flush_cb(lv_display_t *drv, const lv_area_t *area, uint8_t *color_map);

static lv_display_t *g_disp_drv;
static plat_lcd_dev_t *g_lcd_dev;

void lv_port_disp_init(plat_lcd_dev_t *lcd_dev)
{
    assert(lcd_dev);
    g_lcd_dev = lcd_dev;

    // 在 init 之前设置 flush 完成回调，impl 的 init 会读取并传给 bsp
    lcd_dev->flush_done_cb = _lv_port_trans_done_cb;
    lcd_dev->flush_done_cb_arg = NULL;

    g_disp_drv = lv_display_create(lcd_dev->width, lcd_dev->height);
    assert(g_disp_drv);

    // display 创建后，设置回调参数为 display 指针
    lcd_dev->flush_done_cb_arg = g_disp_drv;

    lv_display_set_color_format(g_disp_drv, LV_COLOR_FORMAT_RGB565_SWAPPED);

    size_t buf_size = lcd_dev->height * lcd_dev->width / 10 * sizeof(lv_color_t);
    lv_color_t *buf1 = heap_caps_malloc(buf_size, MALLOC_CAP_DMA);
    assert(buf1);
    lv_color_t *buf2 = heap_caps_malloc(buf_size, MALLOC_CAP_DMA);
    assert(buf2);
    lv_display_set_buffers(g_disp_drv, buf1, buf2, buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_display_set_flush_cb(g_disp_drv, _lv_port_flush_cb);

    // 通过 platform 层初始化 LCD（触发 impl -> bsp 的 init 链路）
    plat_lcd_dev_init(lcd_dev);

    LOG_INFO("lvgl port display driver init");
}

static bool _lv_port_trans_done_cb(void *ctx)
{
    lv_display_t *drv = (lv_display_t *)ctx;
    lv_display_flush_ready(drv);
    return false;
}

static void _lv_port_flush_cb(lv_display_t *drv, const lv_area_t *area, uint8_t *color_map)
{
    plat_lcd_dev_flush(g_lcd_dev, area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_map);
}