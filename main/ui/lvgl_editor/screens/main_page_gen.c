/**
 * @file main_page_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "main_page_gen.h"
#include "../ui.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * main_page_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    static bool style_inited = false;

    if (!style_inited) {

        style_inited = true;
    }

    if (main_page == NULL) main_page = lv_obj_create(NULL);
    lv_obj_t * lv_obj_0 = main_page;
    lv_obj_set_name_static(lv_obj_0, "main_page_#");

    lv_obj_t * func_smart_home = lv_button_create(lv_obj_0);
    lv_obj_set_name(func_smart_home, "func_smart_home");
    lv_obj_set_x(func_smart_home, 60);
    lv_obj_set_y(func_smart_home, 200);
    lv_obj_set_width(func_smart_home, 330);
    lv_obj_set_height(func_smart_home, 150);
    lv_obj_add_style(func_smart_home, &style_func_card, 0);
    lv_obj_t * func_smart_home_icon = lv_label_create(func_smart_home);
    lv_obj_set_name(func_smart_home_icon, "func_smart_home_icon");
    lv_obj_set_width(func_smart_home_icon, LV_SIZE_CONTENT);
    lv_obj_set_height(func_smart_home_icon, LV_SIZE_CONTENT);
    lv_obj_set_style_text_color(func_smart_home_icon, lv_color_hex(0xffffff), 0);

    lv_obj_t * func_smart_home_label = lv_label_create(func_smart_home);
    lv_obj_set_name(func_smart_home_label, "func_smart_home_label");
    lv_obj_set_width(func_smart_home_label, LV_SIZE_CONTENT);
    lv_obj_set_height(func_smart_home_label, LV_SIZE_CONTENT);
    lv_label_set_text(func_smart_home_label, "智能家具");
    lv_obj_set_style_text_color(func_smart_home_label, lv_color_hex(0xffffff), 0);

    lv_obj_add_screen_load_event(func_smart_home, LV_EVENT_CLICKED, smart_home, LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 500, 0);

    lv_obj_t * func_weather = lv_button_create(lv_obj_0);
    lv_obj_set_name(func_weather, "func_weather");
    lv_obj_set_x(func_weather, 60);
    lv_obj_set_y(func_weather, 60);
    lv_obj_set_width(func_weather, 680);
    lv_obj_set_height(func_weather, 110);
    lv_obj_add_style(func_weather, &style_func_card, 0);
    lv_obj_t * func_weather_icon = lv_label_create(func_weather);
    lv_obj_set_name(func_weather_icon, "func_weather_icon");
    lv_obj_set_width(func_weather_icon, LV_SIZE_CONTENT);
    lv_obj_set_height(func_weather_icon, LV_SIZE_CONTENT);
    lv_obj_set_style_text_color(func_weather_icon, lv_color_hex(0xffffff), 0);

    lv_obj_t * func_weather_label = lv_label_create(func_weather);
    lv_obj_set_name(func_weather_label, "func_weather_label");
    lv_obj_set_width(func_weather_label, LV_SIZE_CONTENT);
    lv_obj_set_height(func_weather_label, LV_SIZE_CONTENT);
    lv_label_set_text(func_weather_label, "天气");
    lv_obj_set_style_text_color(func_weather_label, lv_color_hex(0xffffff), 0);

    lv_obj_add_screen_load_event(func_weather, LV_EVENT_CLICKED, weather, LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 500, 0);

    lv_obj_t * func_ai_chat = lv_button_create(lv_obj_0);
    lv_obj_set_name(func_ai_chat, "func_ai_chat");
    lv_obj_set_x(func_ai_chat, 410);
    lv_obj_set_y(func_ai_chat, 200);
    lv_obj_set_width(func_ai_chat, 330);
    lv_obj_set_height(func_ai_chat, 150);
    lv_obj_add_style(func_ai_chat, &style_func_card, 0);
    lv_obj_t * func_ai_chat_icon = lv_label_create(func_ai_chat);
    lv_obj_set_name(func_ai_chat_icon, "func_ai_chat_icon");
    lv_obj_set_width(func_ai_chat_icon, LV_SIZE_CONTENT);
    lv_obj_set_height(func_ai_chat_icon, LV_SIZE_CONTENT);
    lv_obj_set_style_text_color(func_ai_chat_icon, lv_color_hex(0xffffff), 0);

    lv_obj_t * func_ai_chat_label = lv_label_create(func_ai_chat);
    lv_obj_set_name(func_ai_chat_label, "func_ai_chat_label");
    lv_obj_set_width(func_ai_chat_label, LV_SIZE_CONTENT);
    lv_obj_set_height(func_ai_chat_label, LV_SIZE_CONTENT);
    lv_label_set_text(func_ai_chat_label, "AI对话");
    lv_obj_set_style_text_color(func_ai_chat_label, lv_color_hex(0xffffff), 0);

    lv_obj_add_screen_load_event(func_ai_chat, LV_EVENT_CLICKED, ai_chat, LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 500, 0);

    volume_slider_create(lv_obj_0);

    wifi_btn_create(lv_obj_0);

    nav_bar_create(lv_obj_0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

