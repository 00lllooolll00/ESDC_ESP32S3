/**
 * @file weather_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "weather_gen.h"
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

lv_obj_t * weather_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    static bool style_inited = false;

    if (!style_inited) {

        style_inited = true;
    }

    if (weather == NULL) weather = lv_obj_create(NULL);
    lv_obj_t * lv_obj_0 = weather;
    lv_obj_set_name_static(lv_obj_0, "weather_#");

    lv_obj_t * weather_city = lv_label_create(lv_obj_0);
    lv_obj_set_name(weather_city, "weather_city");
    lv_obj_set_x(weather_city, 340);
    lv_obj_set_y(weather_city, 54);
    lv_obj_set_width(weather_city, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_city, LV_SIZE_CONTENT);
    lv_label_set_text(weather_city, "");
    lv_obj_set_style_text_color(weather_city, lv_color_hex(0x212121), 0);
    lv_obj_set_style_text_font(weather_city, chinese_3500_14, 0);

    lv_obj_t * weather_title = lv_label_create(lv_obj_0);
    lv_obj_set_name(weather_title, "weather_title");
    lv_obj_set_x(weather_title, 340);
    lv_obj_set_y(weather_title, 78);
    lv_obj_set_width(weather_title, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_title, LV_SIZE_CONTENT);
    lv_label_set_text(weather_title, "天气查看");
    lv_obj_set_style_text_color(weather_title, lv_color_hex(0x6b7682), 0);
    lv_obj_set_style_text_font(weather_title, chinese_3500_14, 0);

    wifi_btn_create(lv_obj_0);

    volume_slider_create(lv_obj_0);

    nav_bar_create(lv_obj_0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

