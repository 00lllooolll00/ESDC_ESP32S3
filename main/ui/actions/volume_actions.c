#include "common_header.h"
#include "styles.h"
#include "vars.h"
#include "screens.h"

void action_volume_change(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *slider = lv_event_get_target_obj(e);
    int32_t value = lv_slider_get_value(slider);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        set_var_g_volume(value);
        lv_label_set_text(objects.obj1__volume_icon,
                          value ? (value >= 75 ? LV_SYMBOL_VOLUME_MAX : LV_SYMBOL_VOLUME_MID) : LV_SYMBOL_MUTE);
    }
}
