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
    SCREEN_ID_SMART_HOME = 2,
    SCREEN_ID_WEATHER = 3,
    SCREEN_ID_AI_CHAT = 4,
    _SCREEN_ID_LAST = 4
};

typedef struct _objects_t {
    lv_obj_t *main_page;
    lv_obj_t *smart_home;
    lv_obj_t *weather;
    lv_obj_t *ai_chat;
    lv_obj_t *obj0;
    lv_obj_t *obj0__nav_back_btn;
    lv_obj_t *obj0__nav_back_icon;
    lv_obj_t *obj0__nav_home_btn;
    lv_obj_t *obj0__nav_home_icon;
    lv_obj_t *obj1;
    lv_obj_t *obj1__volume_panel;
    lv_obj_t *obj1__volume_canel;
    lv_obj_t *obj1__volume_canel_icon;
    lv_obj_t *obj1__volume_slider;
    lv_obj_t *obj1__obj2;
    lv_obj_t *obj1__volume_icon;
    lv_obj_t *obj2;
    lv_obj_t *obj2__wifi_list_panel;
    lv_obj_t *obj2__wifi_refresh_btn;
    lv_obj_t *obj2__wifi_cancel_btn;
    lv_obj_t *obj2__wifi_list;
    lv_obj_t *obj2__wifi_loader;
    lv_obj_t *obj2__wifi_btn;
    lv_obj_t *obj2__wifi_icon;
    lv_obj_t *obj2__wifi_state;
    lv_obj_t *obj2__obj0;
    lv_obj_t *obj2__obj1;
    lv_obj_t *obj3;
    lv_obj_t *obj3__wifi_list_panel;
    lv_obj_t *obj3__wifi_refresh_btn;
    lv_obj_t *obj3__wifi_cancel_btn;
    lv_obj_t *obj3__wifi_list;
    lv_obj_t *obj3__wifi_loader;
    lv_obj_t *obj3__wifi_btn;
    lv_obj_t *obj3__wifi_icon;
    lv_obj_t *obj3__wifi_state;
    lv_obj_t *obj3__obj0;
    lv_obj_t *obj3__obj1;
    lv_obj_t *obj4;
    lv_obj_t *obj4__volume_panel;
    lv_obj_t *obj4__volume_canel;
    lv_obj_t *obj4__volume_canel_icon;
    lv_obj_t *obj4__volume_slider;
    lv_obj_t *obj4__obj2;
    lv_obj_t *obj4__volume_icon;
    lv_obj_t *obj5;
    lv_obj_t *obj5__nav_back_btn;
    lv_obj_t *obj5__nav_back_icon;
    lv_obj_t *obj5__nav_home_btn;
    lv_obj_t *obj5__nav_home_icon;
    lv_obj_t *obj6;
    lv_obj_t *obj6__wifi_list_panel;
    lv_obj_t *obj6__wifi_refresh_btn;
    lv_obj_t *obj6__wifi_cancel_btn;
    lv_obj_t *obj6__wifi_list;
    lv_obj_t *obj6__wifi_loader;
    lv_obj_t *obj6__wifi_btn;
    lv_obj_t *obj6__wifi_icon;
    lv_obj_t *obj6__wifi_state;
    lv_obj_t *obj6__obj0;
    lv_obj_t *obj6__obj1;
    lv_obj_t *obj7;
    lv_obj_t *obj7__volume_panel;
    lv_obj_t *obj7__volume_canel;
    lv_obj_t *obj7__volume_canel_icon;
    lv_obj_t *obj7__volume_slider;
    lv_obj_t *obj7__obj2;
    lv_obj_t *obj7__volume_icon;
    lv_obj_t *obj8;
    lv_obj_t *obj8__nav_back_btn;
    lv_obj_t *obj8__nav_back_icon;
    lv_obj_t *obj8__nav_home_btn;
    lv_obj_t *obj8__nav_home_icon;
    lv_obj_t *obj9;
    lv_obj_t *obj9__wifi_list_panel;
    lv_obj_t *obj9__wifi_refresh_btn;
    lv_obj_t *obj9__wifi_cancel_btn;
    lv_obj_t *obj9__wifi_list;
    lv_obj_t *obj9__wifi_loader;
    lv_obj_t *obj9__wifi_btn;
    lv_obj_t *obj9__wifi_icon;
    lv_obj_t *obj9__wifi_state;
    lv_obj_t *obj9__obj0;
    lv_obj_t *obj9__obj1;
    lv_obj_t *obj10;
    lv_obj_t *obj10__volume_panel;
    lv_obj_t *obj10__volume_canel;
    lv_obj_t *obj10__volume_canel_icon;
    lv_obj_t *obj10__volume_slider;
    lv_obj_t *obj10__obj2;
    lv_obj_t *obj10__volume_icon;
    lv_obj_t *obj11;
    lv_obj_t *obj11__nav_back_btn;
    lv_obj_t *obj11__nav_back_icon;
    lv_obj_t *obj11__nav_home_btn;
    lv_obj_t *obj11__nav_home_icon;
    lv_obj_t *func_smart_home;
    lv_obj_t *func_smart_home_icon;
    lv_obj_t *func_smart_home_label;
    lv_obj_t *func_weather;
    lv_obj_t *func_weather_icon;
    lv_obj_t *func_weather_label;
    lv_obj_t *func_ai_chat;
    lv_obj_t *func_ai_chat_icon;
    lv_obj_t *func_ai_chat_label;
    lv_obj_t *smart_home_title;
    lv_obj_t *weather_title;
    lv_obj_t *ai_chat_title;
} objects_t;

extern objects_t objects;

void create_screen_main_page();
void tick_screen_main_page();

void create_screen_smart_home();
void tick_screen_smart_home();

void create_screen_weather();
void tick_screen_weather();

void create_screen_ai_chat();
void tick_screen_ai_chat();

void create_user_widget_wifi_btn(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_wifi_btn(void *flowState, int startWidgetIndex);

void create_user_widget_volume_slider(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_volume_slider(void *flowState, int startWidgetIndex);

void create_user_widget_nav_bar(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_nav_bar(void *flowState, int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/