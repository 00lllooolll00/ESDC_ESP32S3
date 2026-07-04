/**
 * @file ui_gen.h
 */

#ifndef UI_GEN_H
#define UI_GEN_H

#ifndef UI_SUBJECT_STRING_LENGTH
#define UI_SUBJECT_STRING_LENGTH 256
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
    #include "lvgl_private.h"
#else
    #include "lvgl/lvgl.h"
    #include "lvgl/lvgl_private.h"
#endif

#ifdef LV_USE_XML
    #include "lv_xml/lv_xml.h"
#endif



/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/*-------------------
 * Permanent screens
 *------------------*/

extern lv_obj_t * ai_chat;
extern lv_obj_t * main_page;
extern lv_obj_t * smart_home;
extern lv_obj_t * weather;

/*----------------
 * Global styles
 *----------------*/

extern lv_style_t style_user_btn;
extern lv_style_t style_user_icon_label;
extern lv_style_t style_user_text_label;
extern lv_style_t style_volume_slider_main;
extern lv_style_t style_volume_slider_knob;
extern lv_style_t style_func_card;
extern lv_style_t style_weather_card;

/*----------------
 * Fonts
 *----------------*/

extern lv_font_t * chinese_3500_14;

/*----------------
 * Images
 *----------------*/

/*----------------
 * Subjects
 *----------------*/

extern lv_subject_t wifi_is_connected;
extern lv_subject_t volume;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*----------------
 * Event Callbacks
 *----------------*/

void nav_back(lv_event_t * e);
void hide_volume_panel(lv_event_t * e);
void action_volume_change(lv_event_t * e);
void show_volume_panel(lv_event_t * e);
void action_wifi_start_scan(lv_event_t * e);
void hide_wifi_panel(lv_event_t * e);
void show_wifi_panel(lv_event_t * e);
void action_weather_refresh(lv_event_t * e);

/**
 * Initialize the component library
 */

void ui_init_gen(const char * asset_path);

/**********************
 *      MACROS
 **********************/

/**********************
 *   POST INCLUDES
 **********************/

/*Include all the widgets, components and screens of this library*/
#include "components/nav_bar_gen.h"
#include "components/volume_slider_gen.h"
#include "components/wifi_btn_gen.h"
#include "screens/ai_chat_gen.h"
#include "screens/main_page_gen.h"
#include "screens/smart_home_gen.h"
#include "screens/weather_gen.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*UI_GEN_H*/