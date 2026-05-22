#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN_PAGE = 1,
    _SCREEN_ID_LAST = 1
};

typedef struct _objects_t {
    lv_obj_t *main_page;
    lv_obj_t *obj0;
    lv_obj_t *obj0__wifi_list_panel;
    lv_obj_t *obj0__wifi_refresh_btn;
    lv_obj_t *obj0__wifi_cancel_btn;
    lv_obj_t *obj0__wifi_list;
    lv_obj_t *obj0__wifi_loader;
    lv_obj_t *obj0__wifi_btn;
    lv_obj_t *obj0__wifi_icon;
    lv_obj_t *obj0__wifi_state;
    lv_obj_t *obj0__obj0;
    lv_obj_t *obj0__obj1;
} objects_t;

extern objects_t objects;

void create_screen_main_page();
void tick_screen_main_page();

void create_user_widget_wifi_btn(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_wifi_btn(void *flowState, int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/