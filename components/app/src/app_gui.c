#include "app_gui.h"
#include "lvgl.h"

void app_gui_init(void)
{
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex3(0xffff), LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello world");
    lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex3(0xf800), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}
