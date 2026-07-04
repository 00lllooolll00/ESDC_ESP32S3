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
    lv_obj_set_style_bg_color(lv_obj_0, lv_color_hex(0x1a2c42), 0);
    lv_obj_set_style_bg_opa(lv_obj_0, 255, 0);

    lv_obj_t * weather_main_box = lv_obj_create(lv_obj_0);
    lv_obj_set_name(weather_main_box, "weather_main_box");
    lv_obj_set_x(weather_main_box, 0);
    lv_obj_set_y(weather_main_box, 48);
    lv_obj_set_width(weather_main_box, 800);
    lv_obj_set_height(weather_main_box, 92);
    lv_obj_set_flag(weather_main_box, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_layout(weather_main_box, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(weather_main_box, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(weather_main_box, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(weather_main_box, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_bg_color(weather_main_box, lv_color_hex(0x1a2c42), 0);
    lv_obj_set_style_bg_opa(weather_main_box, 255, 0);
    lv_obj_set_style_border_width(weather_main_box, 0, 0);
    lv_obj_set_style_pad_left(weather_main_box, 0, 0);
    lv_obj_set_style_pad_top(weather_main_box, 0, 0);
    lv_obj_set_style_pad_right(weather_main_box, 0, 0);
    lv_obj_set_style_pad_bottom(weather_main_box, 0, 0);
    lv_obj_set_style_pad_column(weather_main_box, 16, 0);
    lv_obj_t * weather_temp = lv_label_create(weather_main_box);
    lv_obj_set_name(weather_temp, "weather_temp");
    lv_obj_set_width(weather_temp, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_temp, LV_SIZE_CONTENT);
    lv_label_set_text(weather_temp, "--°");
    lv_obj_set_style_text_color(weather_temp, lv_color_hex(0xff9f43), 0);

    lv_obj_t * lv_obj_1 = lv_obj_create(weather_main_box);
    lv_obj_set_width(lv_obj_1, LV_SIZE_CONTENT);
    lv_obj_set_height(lv_obj_1, LV_SIZE_CONTENT);
    lv_obj_set_style_layout(lv_obj_1, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(lv_obj_1, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(lv_obj_1, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(lv_obj_1, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_bg_opa(lv_obj_1, 0, 0);
    lv_obj_set_style_border_width(lv_obj_1, 0, 0);
    lv_obj_set_style_pad_row(lv_obj_1, 2, 0);
    lv_obj_t * weather_city = lv_label_create(lv_obj_1);
    lv_obj_set_name(weather_city, "weather_city");
    lv_obj_set_width(weather_city, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_city, LV_SIZE_CONTENT);
    lv_label_set_text(weather_city, "");
    lv_obj_set_style_text_color(weather_city, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(weather_city, font_chinese_3500_18, 0);

    lv_obj_t * weather_type = lv_label_create(lv_obj_1);
    lv_obj_set_name(weather_type, "weather_type");
    lv_obj_set_width(weather_type, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_type, LV_SIZE_CONTENT);
    lv_label_set_text(weather_type, "未知");
    lv_obj_set_style_text_color(weather_type, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(weather_type, font_chinese_6500_14, 0);

    lv_obj_t * weather_card_hum = lv_obj_create(lv_obj_0);
    lv_obj_set_name(weather_card_hum, "weather_card_hum");
    lv_obj_set_x(weather_card_hum, 16);
    lv_obj_set_y(weather_card_hum, 148);
    lv_obj_set_width(weather_card_hum, 370);
    lv_obj_set_height(weather_card_hum, 46);
    lv_obj_set_flag(weather_card_hum, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_bg_color(weather_card_hum, lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_bg_opa(weather_card_hum, 255, 0);
    lv_obj_set_style_radius(weather_card_hum, 8, 0);
    lv_obj_set_style_border_width(weather_card_hum, 0, 0);
    lv_obj_set_style_layout(weather_card_hum, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(weather_card_hum, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(weather_card_hum, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_flex_cross_place(weather_card_hum, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_left(weather_card_hum, 12, 0);
    lv_obj_set_style_pad_top(weather_card_hum, 0, 0);
    lv_obj_set_style_pad_right(weather_card_hum, 0, 0);
    lv_obj_set_style_pad_bottom(weather_card_hum, 0, 0);
    lv_obj_set_style_pad_column(weather_card_hum, 8, 0);
    lv_obj_t * weather_ic_hum = lv_image_create(weather_card_hum);
    lv_obj_set_name(weather_ic_hum, "weather_ic_hum");
    lv_obj_set_width(weather_ic_hum, 24);
    lv_obj_set_height(weather_ic_hum, 24);

    lv_obj_t * lv_obj_2 = lv_obj_create(weather_card_hum);
    lv_obj_set_width(lv_obj_2, LV_SIZE_CONTENT);
    lv_obj_set_height(lv_obj_2, LV_SIZE_CONTENT);
    lv_obj_set_style_layout(lv_obj_2, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(lv_obj_2, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(lv_obj_2, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(lv_obj_2, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_bg_opa(lv_obj_2, 0, 0);
    lv_obj_set_style_border_width(lv_obj_2, 0, 0);
    lv_obj_set_style_pad_left(lv_obj_2, 0, 0);
    lv_obj_set_style_pad_top(lv_obj_2, 0, 0);
    lv_obj_set_style_pad_right(lv_obj_2, 0, 0);
    lv_obj_set_style_pad_bottom(lv_obj_2, 0, 0);
    lv_obj_t * weather_humidity_lbl = lv_label_create(lv_obj_2);
    lv_obj_set_name(weather_humidity_lbl, "weather_humidity_lbl");
    lv_obj_set_width(weather_humidity_lbl, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_humidity_lbl, LV_SIZE_CONTENT);
    lv_label_set_text(weather_humidity_lbl, "湿度");
    lv_obj_set_style_text_color(weather_humidity_lbl, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(weather_humidity_lbl, font_chinese_6500_14, 0);

    lv_obj_t * weather_humidity = lv_label_create(lv_obj_2);
    lv_obj_set_name(weather_humidity, "weather_humidity");
    lv_obj_set_width(weather_humidity, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_humidity, LV_SIZE_CONTENT);
    lv_label_set_text(weather_humidity, "--%");
    lv_obj_set_style_text_color(weather_humidity, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(weather_humidity, font_chinese_6500_14, 0);

    lv_obj_t * weather_card_wind = lv_obj_create(lv_obj_0);
    lv_obj_set_name(weather_card_wind, "weather_card_wind");
    lv_obj_set_x(weather_card_wind, 414);
    lv_obj_set_y(weather_card_wind, 148);
    lv_obj_set_width(weather_card_wind, 370);
    lv_obj_set_height(weather_card_wind, 46);
    lv_obj_set_flag(weather_card_wind, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_bg_color(weather_card_wind, lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_bg_opa(weather_card_wind, 255, 0);
    lv_obj_set_style_radius(weather_card_wind, 8, 0);
    lv_obj_set_style_border_width(weather_card_wind, 0, 0);
    lv_obj_set_style_layout(weather_card_wind, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(weather_card_wind, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(weather_card_wind, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_flex_cross_place(weather_card_wind, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_left(weather_card_wind, 12, 0);
    lv_obj_set_style_pad_top(weather_card_wind, 0, 0);
    lv_obj_set_style_pad_right(weather_card_wind, 0, 0);
    lv_obj_set_style_pad_bottom(weather_card_wind, 0, 0);
    lv_obj_set_style_pad_column(weather_card_wind, 8, 0);
    lv_obj_t * weather_ic_wind = lv_image_create(weather_card_wind);
    lv_obj_set_name(weather_ic_wind, "weather_ic_wind");
    lv_obj_set_width(weather_ic_wind, 24);
    lv_obj_set_height(weather_ic_wind, 24);

    lv_obj_t * lv_obj_3 = lv_obj_create(weather_card_wind);
    lv_obj_set_width(lv_obj_3, LV_SIZE_CONTENT);
    lv_obj_set_height(lv_obj_3, LV_SIZE_CONTENT);
    lv_obj_set_style_layout(lv_obj_3, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(lv_obj_3, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(lv_obj_3, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(lv_obj_3, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_bg_opa(lv_obj_3, 0, 0);
    lv_obj_set_style_border_width(lv_obj_3, 0, 0);
    lv_obj_set_style_pad_left(lv_obj_3, 0, 0);
    lv_obj_set_style_pad_top(lv_obj_3, 0, 0);
    lv_obj_set_style_pad_right(lv_obj_3, 0, 0);
    lv_obj_set_style_pad_bottom(lv_obj_3, 0, 0);
    lv_obj_t * weather_wind_lbl = lv_label_create(lv_obj_3);
    lv_obj_set_name(weather_wind_lbl, "weather_wind_lbl");
    lv_obj_set_width(weather_wind_lbl, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_wind_lbl, LV_SIZE_CONTENT);
    lv_label_set_text(weather_wind_lbl, "风速");
    lv_obj_set_style_text_color(weather_wind_lbl, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(weather_wind_lbl, font_chinese_6500_14, 0);

    lv_obj_t * weather_wind = lv_label_create(lv_obj_3);
    lv_obj_set_name(weather_wind, "weather_wind");
    lv_obj_set_width(weather_wind, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_wind, LV_SIZE_CONTENT);
    lv_label_set_text(weather_wind, "--km/h");
    lv_obj_set_style_text_color(weather_wind, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(weather_wind, font_chinese_6500_14, 0);

    lv_obj_t * weather_card_max = lv_obj_create(lv_obj_0);
    lv_obj_set_name(weather_card_max, "weather_card_max");
    lv_obj_set_x(weather_card_max, 16);
    lv_obj_set_y(weather_card_max, 200);
    lv_obj_set_width(weather_card_max, 370);
    lv_obj_set_height(weather_card_max, 46);
    lv_obj_set_flag(weather_card_max, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_bg_color(weather_card_max, lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_bg_opa(weather_card_max, 255, 0);
    lv_obj_set_style_radius(weather_card_max, 8, 0);
    lv_obj_set_style_border_width(weather_card_max, 0, 0);
    lv_obj_set_style_layout(weather_card_max, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(weather_card_max, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(weather_card_max, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_flex_cross_place(weather_card_max, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_left(weather_card_max, 12, 0);
    lv_obj_set_style_pad_top(weather_card_max, 0, 0);
    lv_obj_set_style_pad_right(weather_card_max, 0, 0);
    lv_obj_set_style_pad_bottom(weather_card_max, 0, 0);
    lv_obj_set_style_pad_column(weather_card_max, 8, 0);
    lv_obj_t * weather_ic_max = lv_image_create(weather_card_max);
    lv_obj_set_name(weather_ic_max, "weather_ic_max");
    lv_obj_set_width(weather_ic_max, 24);
    lv_obj_set_height(weather_ic_max, 24);

    lv_obj_t * lv_obj_4 = lv_obj_create(weather_card_max);
    lv_obj_set_width(lv_obj_4, LV_SIZE_CONTENT);
    lv_obj_set_height(lv_obj_4, LV_SIZE_CONTENT);
    lv_obj_set_style_layout(lv_obj_4, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(lv_obj_4, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(lv_obj_4, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(lv_obj_4, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_bg_opa(lv_obj_4, 0, 0);
    lv_obj_set_style_border_width(lv_obj_4, 0, 0);
    lv_obj_set_style_pad_left(lv_obj_4, 0, 0);
    lv_obj_set_style_pad_top(lv_obj_4, 0, 0);
    lv_obj_set_style_pad_right(lv_obj_4, 0, 0);
    lv_obj_set_style_pad_bottom(lv_obj_4, 0, 0);
    lv_obj_t * weather_max_label = lv_label_create(lv_obj_4);
    lv_obj_set_name(weather_max_label, "weather_max_label");
    lv_obj_set_width(weather_max_label, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_max_label, LV_SIZE_CONTENT);
    lv_label_set_text(weather_max_label, "最高温");
    lv_obj_set_style_text_color(weather_max_label, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(weather_max_label, font_chinese_6500_14, 0);

    lv_obj_t * weather_max_temp = lv_label_create(lv_obj_4);
    lv_obj_set_name(weather_max_temp, "weather_max_temp");
    lv_obj_set_width(weather_max_temp, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_max_temp, LV_SIZE_CONTENT);
    lv_label_set_text(weather_max_temp, "--°");
    lv_obj_set_style_text_color(weather_max_temp, lv_color_hex(0xff6b6b), 0);

    lv_obj_t * weather_card_min = lv_obj_create(lv_obj_0);
    lv_obj_set_name(weather_card_min, "weather_card_min");
    lv_obj_set_x(weather_card_min, 414);
    lv_obj_set_y(weather_card_min, 200);
    lv_obj_set_width(weather_card_min, 370);
    lv_obj_set_height(weather_card_min, 46);
    lv_obj_set_flag(weather_card_min, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_bg_color(weather_card_min, lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_bg_opa(weather_card_min, 255, 0);
    lv_obj_set_style_radius(weather_card_min, 8, 0);
    lv_obj_set_style_border_width(weather_card_min, 0, 0);
    lv_obj_set_style_layout(weather_card_min, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(weather_card_min, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(weather_card_min, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_flex_cross_place(weather_card_min, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_left(weather_card_min, 12, 0);
    lv_obj_set_style_pad_top(weather_card_min, 0, 0);
    lv_obj_set_style_pad_right(weather_card_min, 0, 0);
    lv_obj_set_style_pad_bottom(weather_card_min, 0, 0);
    lv_obj_set_style_pad_column(weather_card_min, 8, 0);
    lv_obj_t * weather_ic_min = lv_image_create(weather_card_min);
    lv_obj_set_name(weather_ic_min, "weather_ic_min");
    lv_obj_set_width(weather_ic_min, 24);
    lv_obj_set_height(weather_ic_min, 24);

    lv_obj_t * lv_obj_5 = lv_obj_create(weather_card_min);
    lv_obj_set_width(lv_obj_5, LV_SIZE_CONTENT);
    lv_obj_set_height(lv_obj_5, LV_SIZE_CONTENT);
    lv_obj_set_style_layout(lv_obj_5, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(lv_obj_5, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(lv_obj_5, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(lv_obj_5, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_bg_opa(lv_obj_5, 0, 0);
    lv_obj_set_style_border_width(lv_obj_5, 0, 0);
    lv_obj_set_style_pad_left(lv_obj_5, 0, 0);
    lv_obj_set_style_pad_top(lv_obj_5, 0, 0);
    lv_obj_set_style_pad_right(lv_obj_5, 0, 0);
    lv_obj_set_style_pad_bottom(lv_obj_5, 0, 0);
    lv_obj_t * weather_min_label = lv_label_create(lv_obj_5);
    lv_obj_set_name(weather_min_label, "weather_min_label");
    lv_obj_set_width(weather_min_label, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_min_label, LV_SIZE_CONTENT);
    lv_label_set_text(weather_min_label, "最低温");
    lv_obj_set_style_text_color(weather_min_label, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(weather_min_label, font_chinese_6500_14, 0);

    lv_obj_t * weather_min_temp = lv_label_create(lv_obj_5);
    lv_obj_set_name(weather_min_temp, "weather_min_temp");
    lv_obj_set_width(weather_min_temp, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_min_temp, LV_SIZE_CONTENT);
    lv_label_set_text(weather_min_temp, "--°");
    lv_obj_set_style_text_color(weather_min_temp, lv_color_hex(0x4dabf7), 0);

    lv_obj_t * weather_daily_card_0 = lv_obj_create(lv_obj_0);
    lv_obj_set_name(weather_daily_card_0, "weather_daily_card_0");
    lv_obj_set_x(weather_daily_card_0, 16);
    lv_obj_set_y(weather_daily_card_0, 252);
    lv_obj_set_width(weather_daily_card_0, 252);
    lv_obj_set_height(weather_daily_card_0, 50);
    lv_obj_set_flag(weather_daily_card_0, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_bg_color(weather_daily_card_0, lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_bg_opa(weather_daily_card_0, 255, 0);
    lv_obj_set_style_radius(weather_daily_card_0, 8, 0);
    lv_obj_set_style_border_width(weather_daily_card_0, 0, 0);
    lv_obj_set_style_layout(weather_daily_card_0, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(weather_daily_card_0, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(weather_daily_card_0, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(weather_daily_card_0, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_left(weather_daily_card_0, 4, 0);
    lv_obj_set_style_pad_top(weather_daily_card_0, 4, 0);
    lv_obj_set_style_pad_right(weather_daily_card_0, 4, 0);
    lv_obj_set_style_pad_bottom(weather_daily_card_0, 4, 0);

    lv_obj_t * weather_daily_card_1 = lv_obj_create(lv_obj_0);
    lv_obj_set_name(weather_daily_card_1, "weather_daily_card_1");
    lv_obj_set_x(weather_daily_card_1, 274);
    lv_obj_set_y(weather_daily_card_1, 252);
    lv_obj_set_width(weather_daily_card_1, 252);
    lv_obj_set_height(weather_daily_card_1, 50);
    lv_obj_set_flag(weather_daily_card_1, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_bg_color(weather_daily_card_1, lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_bg_opa(weather_daily_card_1, 255, 0);
    lv_obj_set_style_radius(weather_daily_card_1, 8, 0);
    lv_obj_set_style_border_width(weather_daily_card_1, 0, 0);
    lv_obj_set_style_layout(weather_daily_card_1, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(weather_daily_card_1, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(weather_daily_card_1, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(weather_daily_card_1, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_left(weather_daily_card_1, 4, 0);
    lv_obj_set_style_pad_top(weather_daily_card_1, 4, 0);
    lv_obj_set_style_pad_right(weather_daily_card_1, 4, 0);
    lv_obj_set_style_pad_bottom(weather_daily_card_1, 4, 0);

    lv_obj_t * weather_daily_card_2 = lv_obj_create(lv_obj_0);
    lv_obj_set_name(weather_daily_card_2, "weather_daily_card_2");
    lv_obj_set_x(weather_daily_card_2, 532);
    lv_obj_set_y(weather_daily_card_2, 252);
    lv_obj_set_width(weather_daily_card_2, 252);
    lv_obj_set_height(weather_daily_card_2, 50);
    lv_obj_set_flag(weather_daily_card_2, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_bg_color(weather_daily_card_2, lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_bg_opa(weather_daily_card_2, 255, 0);
    lv_obj_set_style_radius(weather_daily_card_2, 8, 0);
    lv_obj_set_style_border_width(weather_daily_card_2, 0, 0);
    lv_obj_set_style_layout(weather_daily_card_2, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(weather_daily_card_2, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(weather_daily_card_2, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(weather_daily_card_2, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_left(weather_daily_card_2, 4, 0);
    lv_obj_set_style_pad_top(weather_daily_card_2, 4, 0);
    lv_obj_set_style_pad_right(weather_daily_card_2, 4, 0);
    lv_obj_set_style_pad_bottom(weather_daily_card_2, 4, 0);

    lv_obj_t * weather_index_card = lv_obj_create(lv_obj_0);
    lv_obj_set_name(weather_index_card, "weather_index_card");
    lv_obj_set_x(weather_index_card, 16);
    lv_obj_set_y(weather_index_card, 308);
    lv_obj_set_width(weather_index_card, 768);
    lv_obj_set_height(weather_index_card, 116);
    lv_obj_set_flag(weather_index_card, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_bg_color(weather_index_card, lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_bg_opa(weather_index_card, 255, 0);
    lv_obj_set_style_radius(weather_index_card, 8, 0);
    lv_obj_set_style_border_width(weather_index_card, 0, 0);
    lv_obj_set_style_layout(weather_index_card, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(weather_index_card, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(weather_index_card, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_flex_cross_place(weather_index_card, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_pad_left(weather_index_card, 16, 0);
    lv_obj_set_style_pad_top(weather_index_card, 8, 0);
    lv_obj_set_style_pad_right(weather_index_card, 16, 0);
    lv_obj_set_style_pad_bottom(weather_index_card, 8, 0);
    lv_obj_set_style_pad_row(weather_index_card, 6, 0);
    lv_obj_t * weather_advice_title = lv_obj_create(weather_index_card);
    lv_obj_set_name(weather_advice_title, "weather_advice_title");
    lv_obj_set_width(weather_advice_title, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_advice_title, LV_SIZE_CONTENT);
    lv_obj_set_flag(weather_advice_title, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_layout(weather_advice_title, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(weather_advice_title, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(weather_advice_title, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_flex_cross_place(weather_advice_title, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_bg_opa(weather_advice_title, 0, 0);
    lv_obj_set_style_border_width(weather_advice_title, 0, 0);
    lv_obj_set_style_pad_column(weather_advice_title, 6, 0);
    lv_obj_t * weather_ic_advice_img = lv_image_create(weather_advice_title);
    lv_obj_set_name(weather_ic_advice_img, "weather_ic_advice_img");
    lv_obj_set_width(weather_ic_advice_img, 20);
    lv_obj_set_height(weather_ic_advice_img, 20);

    lv_obj_t * weather_advice_title_lbl = lv_label_create(weather_advice_title);
    lv_obj_set_name(weather_advice_title_lbl, "weather_advice_title_lbl");
    lv_obj_set_width(weather_advice_title_lbl, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_advice_title_lbl, LV_SIZE_CONTENT);
    lv_label_set_text(weather_advice_title_lbl, "AI 建议");
    lv_obj_set_style_text_color(weather_advice_title_lbl, lv_color_hex(0xff9f43), 0);
    lv_obj_set_style_text_font(weather_advice_title_lbl, font_chinese_6500_14, 0);

    wifi_btn_create(lv_obj_0);

    volume_slider_create(lv_obj_0);

    nav_bar_create(lv_obj_0);

    lv_obj_t * weather_refresh_btn = lv_button_create(lv_obj_0);
    lv_obj_set_name(weather_refresh_btn, "weather_refresh_btn");
    lv_obj_set_x(weather_refresh_btn, 655);
    lv_obj_set_y(weather_refresh_btn, 0);
    lv_obj_set_width(weather_refresh_btn, 70);
    lv_obj_set_height(weather_refresh_btn, 40);
    lv_obj_add_style(weather_refresh_btn, &style_user_btn, 0);
    lv_obj_t * weather_refresh_icon = lv_label_create(weather_refresh_btn);
    lv_obj_set_name(weather_refresh_icon, "weather_refresh_icon");
    lv_obj_set_width(weather_refresh_icon, LV_SIZE_CONTENT);
    lv_obj_set_height(weather_refresh_icon, LV_SIZE_CONTENT);
    lv_obj_add_style(weather_refresh_icon, &style_user_icon_label, 0);

    lv_obj_add_event_cb(weather_refresh_btn, action_weather_refresh, LV_EVENT_CLICKED, NULL);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

