#include "lv_disp_port.h"
#include "bsp_lcd.h"
#include "ek_export.h"

#define LV_PORT_BUF_SIZE (BSP_LCD_BUFFER_SIZE / 10)

static bool _lv_port_trans_done_cb(void *ctx);
static void _lv_port_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);

static lv_disp_draw_buf_t g_buf;
static lv_disp_drv_t g_disp_drv;

void lv_port_disp_init(void)
{
    lv_color_t *buf1 = heap_caps_malloc(LV_PORT_BUF_SIZE, MALLOC_CAP_DMA);
    assert(buf1);
    lv_color_t *buf2 = heap_caps_malloc(LV_PORT_BUF_SIZE, MALLOC_CAP_DMA);
    assert(buf2);

    lv_disp_draw_buf_init(&g_buf, buf1, buf2, LV_PORT_BUF_SIZE);
    g_disp_drv.hor_res = BSP_LCD_WIDTH;
    g_disp_drv.ver_res = BSP_LCD_HEIGHT;
    g_disp_drv.flush_cb = _lv_port_flush_cb;
    g_disp_drv.draw_buf = &g_buf;

    bsp_lcd_init(_lv_port_trans_done_cb, &g_disp_drv);

    lv_disp_drv_register(&g_disp_drv);
}

EK_EXPORT_COMPONENTS(lv_port_disp_init);

static bool _lv_port_trans_done_cb(void *ctx)
{
    lv_disp_drv_t *drv = (lv_disp_drv_t *)ctx;
    lv_disp_flush_ready(drv);
    return false;
}

static void _lv_port_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    bsp_lcd_disp_flush(area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_map);
}
