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

// Style: wifi_scan_result
lv_style_t *get_style_wifi_scan_result_MAIN_DEFAULT();
void add_style_wifi_scan_result(lv_obj_t *obj);
void remove_style_wifi_scan_result(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/