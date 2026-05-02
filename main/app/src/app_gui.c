#include "app_gui.h"
#include "lvgl.h"
#include "lv_demos.h"

void app_gui_init(void)
{
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_make(255, 255, 255), LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello world");
    lv_obj_set_style_text_color(lv_screen_active(), lv_color_make(0, 255, 0), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}
