#include "common_header.h"
#include "ui.h"

void action_volume_change(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *slider = lv_event_get_target_obj(e);
    int32_t value = lv_slider_get_value(slider);
    if (code != LV_EVENT_VALUE_CHANGED)
    {
        return;
    }

    lv_subject_set_int(&volume, value);

    // 同步所有屏幕的 slider 值和 icon 图标
    lv_obj_t *screens[] = {main_page, weather, smart_home, ai_chat};
    const char *icon_text = value ? (value >= 75 ? LV_SYMBOL_VOLUME_MAX : LV_SYMBOL_VOLUME_MID) : LV_SYMBOL_MUTE;
    for (int i = 0; i < 4; i++)
    {
        lv_obj_t *s = lv_obj_find_by_name(screens[i], "volume_slider");
        if (s && s != slider)
        {
            lv_slider_set_value(s, value, LV_ANIM_OFF);
        }
        lv_obj_t *icon = lv_obj_find_by_name(screens[i], "volume_icon");
        if (icon)
        {
            lv_label_set_text(icon, icon_text);
        }
    }
}
