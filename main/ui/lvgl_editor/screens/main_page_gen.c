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
    lv_obj_set_style_bg_color(lv_obj_0, lv_color_hex(0x1a2c42), 0);
    lv_obj_set_style_bg_opa(lv_obj_0, 255, 0);

    lv_obj_t * main_clock = lv_label_create(lv_obj_0);
    lv_obj_set_name(main_clock, "main_clock");
    lv_obj_set_x(main_clock, 300);
    lv_obj_set_y(main_clock, 0);
    lv_obj_set_width(main_clock, 200);
    lv_obj_set_height(main_clock, 36);
    lv_label_set_text(main_clock, "--:--");
    lv_obj_set_style_text_color(main_clock, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_align(main_clock, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_t * main_date = lv_label_create(lv_obj_0);
    lv_obj_set_name(main_date, "main_date");
    lv_obj_set_x(main_date, 300);
    lv_obj_set_y(main_date, 36);
    lv_obj_set_width(main_date, 200);
    lv_obj_set_height(main_date, 20);
    lv_label_set_text(main_date, "");
    lv_obj_set_style_text_color(main_date, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(main_date, chinese_3500_14, 0);
    lv_obj_set_style_text_align(main_date, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_t * func_weather = lv_button_create(lv_obj_0);
    lv_obj_set_name(func_weather, "func_weather");
    lv_obj_set_x(func_weather, 60);
    lv_obj_set_y(func_weather, 60);
    lv_obj_set_width(func_weather, 680);
    lv_obj_set_height(func_weather, 110);
    lv_obj_set_style_layout(func_weather, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(func_weather, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(func_weather, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
    lv_obj_set_style_flex_cross_place(func_weather, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_left(func_weather, 24, 0);
    lv_obj_set_style_pad_right(func_weather, 24, 0);
    lv_obj_add_style(func_weather, &style_func_card, 0);
    lv_obj_t * func_weather_icon = lv_image_create(func_weather);
    lv_obj_set_name(func_weather_icon, "func_weather_icon");
    lv_obj_set_width(func_weather_icon, 48);
    lv_obj_set_height(func_weather_icon, 48);

    lv_obj_t * main_weather_city = lv_label_create(func_weather);
    lv_obj_set_name(main_weather_city, "main_weather_city");
    lv_obj_set_width(main_weather_city, LV_SIZE_CONTENT);
    lv_obj_set_height(main_weather_city, LV_SIZE_CONTENT);
    lv_label_set_text(main_weather_city, "定位中...");
    lv_obj_set_style_text_color(main_weather_city, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(main_weather_city, chinese_3500_14, 0);

    lv_obj_t * main_weather_temp = lv_label_create(func_weather);
    lv_obj_set_name(main_weather_temp, "main_weather_temp");
    lv_obj_set_width(main_weather_temp, LV_SIZE_CONTENT);
    lv_obj_set_height(main_weather_temp, LV_SIZE_CONTENT);
    lv_label_set_text(main_weather_temp, "--°");
    lv_obj_set_style_text_color(main_weather_temp, lv_color_hex(0xff9f43), 0);

    lv_obj_t * main_weather_type = lv_label_create(func_weather);
    lv_obj_set_name(main_weather_type, "main_weather_type");
    lv_obj_set_width(main_weather_type, LV_SIZE_CONTENT);
    lv_obj_set_height(main_weather_type, LV_SIZE_CONTENT);
    lv_label_set_text(main_weather_type, "--");
    lv_obj_set_style_text_color(main_weather_type, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(main_weather_type, chinese_3500_14, 0);

    lv_obj_add_screen_load_event(func_weather, LV_EVENT_CLICKED, weather, LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 500, 0);

    lv_obj_t * func_smart_home = lv_button_create(lv_obj_0);
    lv_obj_set_name(func_smart_home, "func_smart_home");
    lv_obj_set_x(func_smart_home, 60);
    lv_obj_set_y(func_smart_home, 200);
    lv_obj_set_width(func_smart_home, 330);
    lv_obj_set_height(func_smart_home, 150);
    lv_obj_set_style_flex_flow(func_smart_home, LV_FLEX_FLOW_COLUMN, 0);
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
    lv_obj_set_style_text_font(func_smart_home_label, chinese_3500_14, 0);

    lv_obj_t * func_smart_home_desc = lv_label_create(func_smart_home);
    lv_obj_set_name(func_smart_home_desc, "func_smart_home_desc");
    lv_obj_set_width(func_smart_home_desc, LV_SIZE_CONTENT);
    lv_obj_set_height(func_smart_home_desc, LV_SIZE_CONTENT);
    lv_label_set_text(func_smart_home_desc, "灯光或传感器");
    lv_obj_set_style_text_color(func_smart_home_desc, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(func_smart_home_desc, chinese_3500_14, 0);

    lv_obj_add_screen_load_event(func_smart_home, LV_EVENT_CLICKED, smart_home, LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 500, 0);

    lv_obj_t * func_ai_chat = lv_button_create(lv_obj_0);
    lv_obj_set_name(func_ai_chat, "func_ai_chat");
    lv_obj_set_x(func_ai_chat, 410);
    lv_obj_set_y(func_ai_chat, 200);
    lv_obj_set_width(func_ai_chat, 330);
    lv_obj_set_height(func_ai_chat, 150);
    lv_obj_set_style_flex_flow(func_ai_chat, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_add_style(func_ai_chat, &style_func_card, 0);
    lv_obj_t * func_ai_chat_icon = lv_image_create(func_ai_chat);
    lv_obj_set_name(func_ai_chat_icon, "func_ai_chat_icon");
    lv_obj_set_width(func_ai_chat_icon, 36);
    lv_obj_set_height(func_ai_chat_icon, 36);

    lv_obj_t * func_ai_chat_label = lv_label_create(func_ai_chat);
    lv_obj_set_name(func_ai_chat_label, "func_ai_chat_label");
    lv_obj_set_width(func_ai_chat_label, LV_SIZE_CONTENT);
    lv_obj_set_height(func_ai_chat_label, LV_SIZE_CONTENT);
    lv_label_set_text(func_ai_chat_label, "AI对话");
    lv_obj_set_style_text_color(func_ai_chat_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(func_ai_chat_label, chinese_3500_14, 0);

    lv_obj_t * func_ai_chat_desc = lv_label_create(func_ai_chat);
    lv_obj_set_name(func_ai_chat_desc, "func_ai_chat_desc");
    lv_obj_set_width(func_ai_chat_desc, LV_SIZE_CONTENT);
    lv_obj_set_height(func_ai_chat_desc, LV_SIZE_CONTENT);
    lv_label_set_text(func_ai_chat_desc, "智能语音助手");
    lv_obj_set_style_text_color(func_ai_chat_desc, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(func_ai_chat_desc, chinese_3500_14, 0);

    lv_obj_add_screen_load_event(func_ai_chat, LV_EVENT_CLICKED, ai_chat, LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 500, 0);

    lv_obj_t * main_status_bar = lv_obj_create(lv_obj_0);
    lv_obj_set_name(main_status_bar, "main_status_bar");
    lv_obj_set_x(main_status_bar, 60);
    lv_obj_set_y(main_status_bar, 370);
    lv_obj_set_width(main_status_bar, 680);
    lv_obj_set_height(main_status_bar, 40);
    lv_obj_set_flag(main_status_bar, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_bg_color(main_status_bar, lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_bg_opa(main_status_bar, 255, 0);
    lv_obj_set_style_radius(main_status_bar, 8, 0);
    lv_obj_set_style_border_width(main_status_bar, 0, 0);
    lv_obj_set_style_layout(main_status_bar, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(main_status_bar, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(main_status_bar, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
    lv_obj_set_style_flex_cross_place(main_status_bar, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_left(main_status_bar, 16, 0);
    lv_obj_set_style_pad_right(main_status_bar, 16, 0);
    lv_obj_t * main_wifi_ssid = lv_label_create(main_status_bar);
    lv_obj_set_name(main_wifi_ssid, "main_wifi_ssid");
    lv_obj_set_width(main_wifi_ssid, LV_SIZE_CONTENT);
    lv_obj_set_height(main_wifi_ssid, LV_SIZE_CONTENT);
    lv_label_set_text(main_wifi_ssid, "WiFi: --");
    lv_obj_set_style_text_color(main_wifi_ssid, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(main_wifi_ssid, chinese_3500_14, 0);

    lv_obj_t * main_ip_addr = lv_label_create(main_status_bar);
    lv_obj_set_name(main_ip_addr, "main_ip_addr");
    lv_obj_set_width(main_ip_addr, LV_SIZE_CONTENT);
    lv_obj_set_height(main_ip_addr, LV_SIZE_CONTENT);
    lv_label_set_text(main_ip_addr, "IP: --");
    lv_obj_set_style_text_color(main_ip_addr, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(main_ip_addr, chinese_3500_14, 0);

    volume_slider_create(lv_obj_0);

    wifi_btn_create(lv_obj_0);

    nav_bar_create(lv_obj_0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

