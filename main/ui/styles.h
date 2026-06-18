#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: user_btn
lv_style_t *get_style_user_btn_MAIN_DEFAULT();
void add_style_user_btn(lv_obj_t *obj);
void remove_style_user_btn(lv_obj_t *obj);

// Style: wifi_loadder
void add_style_wifi_loadder(lv_obj_t *obj);
void remove_style_wifi_loadder(lv_obj_t *obj);

// Style: user_icon_label
lv_style_t *get_style_user_icon_label_MAIN_DEFAULT();
void add_style_user_icon_label(lv_obj_t *obj);
void remove_style_user_icon_label(lv_obj_t *obj);

// Style: user_text_label
lv_style_t *get_style_user_text_label_MAIN_DEFAULT();
void add_style_user_text_label(lv_obj_t *obj);
void remove_style_user_text_label(lv_obj_t *obj);

// Style: volume_slider
lv_style_t *get_style_volume_slider_MAIN_DEFAULT();
lv_style_t *get_style_volume_slider_KNOB_DEFAULT();
void add_style_volume_slider(lv_obj_t *obj);
void remove_style_volume_slider(lv_obj_t *obj);

// Style: func_card
lv_style_t *get_style_func_card_MAIN_DEFAULT();
void add_style_func_card(lv_obj_t *obj);
void remove_style_func_card(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/