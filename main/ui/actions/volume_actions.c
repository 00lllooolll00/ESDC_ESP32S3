#include "common_header.h"
#include "ui.h"

void action_volume_change(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *slider = lv_event_get_target_obj(e);
    int32_t value = lv_slider_get_value(slider);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_subject_set_int(&volume, value);
        lv_obj_t *icon = lv_obj_find_by_name(lv_screen_active(), "volume_icon");
        if (icon)
        {
            lv_label_set_text(icon, value ? (value >= 75 ? LV_SYMBOL_VOLUME_MAX : LV_SYMBOL_VOLUME_MID) : LV_SYMBOL_MUTE);
        }
    }
}
