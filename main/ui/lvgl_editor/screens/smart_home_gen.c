/**
 * @file smart_home_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "smart_home_gen.h"
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

lv_obj_t * smart_home_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    static bool style_inited = false;

    if (!style_inited) {

        style_inited = true;
    }

    if (smart_home == NULL) smart_home = lv_obj_create(NULL);
    lv_obj_t * lv_obj_0 = smart_home;
    lv_obj_set_name_static(lv_obj_0, "smart_home_#");
    lv_obj_set_style_bg_color(lv_obj_0, lv_color_hex(0x1a2c42), 0);
    lv_obj_set_style_bg_opa(lv_obj_0, 255, 0);

    lv_obj_t * smart_home_title = lv_label_create(lv_obj_0);
    lv_obj_set_name(smart_home_title, "smart_home_title");
    lv_obj_set_x(smart_home_title, 300);
    lv_obj_set_y(smart_home_title, 10);
    lv_obj_set_width(smart_home_title, 200);
    lv_obj_set_height(smart_home_title, 24);
    lv_label_set_text(smart_home_title, "智能家具");
    lv_obj_set_style_text_color(smart_home_title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(smart_home_title, font_chinese_6500_14, 0);
    lv_obj_set_style_text_align(smart_home_title, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_t * sh_scroll_cont = lv_obj_create(lv_obj_0);
    lv_obj_set_name(sh_scroll_cont, "sh_scroll_cont");
    lv_obj_set_x(sh_scroll_cont, 0);
    lv_obj_set_y(sh_scroll_cont, 50);
    lv_obj_set_width(sh_scroll_cont, 800);
    lv_obj_set_height(sh_scroll_cont, 370);
    lv_obj_set_flag(sh_scroll_cont, LV_OBJ_FLAG_SCROLLABLE, true);
    lv_obj_set_style_bg_opa(sh_scroll_cont, 0, 0);
    lv_obj_set_style_border_width(sh_scroll_cont, 0, 0);
    lv_obj_set_style_pad_left(sh_scroll_cont, 0, 0);
    lv_obj_set_style_pad_top(sh_scroll_cont, 0, 0);
    lv_obj_set_style_pad_right(sh_scroll_cont, 0, 0);
    lv_obj_set_style_pad_bottom(sh_scroll_cont, 0, 0);
    lv_obj_t * sh_btn_0 = lv_button_create(sh_scroll_cont);
    lv_obj_set_name(sh_btn_0, "sh_btn_0");
    lv_obj_set_x(sh_btn_0, 20);
    lv_obj_set_y(sh_btn_0, 0);
    lv_obj_set_width(sh_btn_0, 370);
    lv_obj_set_height(sh_btn_0, 120);
    lv_obj_set_style_bg_color(sh_btn_0, lv_color_hex(0x3a5066), 0);
    lv_obj_set_style_bg_opa(sh_btn_0, 255, 0);
    lv_obj_set_style_radius(sh_btn_0, 12, 0);
    lv_obj_set_style_border_width(sh_btn_0, 0, 0);
    lv_obj_set_style_layout(sh_btn_0, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(sh_btn_0, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(sh_btn_0, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(sh_btn_0, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_t * sh_lbl_0 = lv_label_create(sh_btn_0);
    lv_obj_set_name(sh_lbl_0, "sh_lbl_0");
    lv_obj_set_width(sh_lbl_0, LV_SIZE_CONTENT);
    lv_obj_set_height(sh_lbl_0, LV_SIZE_CONTENT);
    lv_label_set_text(sh_lbl_0, "IO1");
    lv_obj_set_style_text_color(sh_lbl_0, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(sh_lbl_0, font_chinese_6500_14, 0);

    lv_obj_add_event_cb(sh_btn_0, action_sh_btn_click, LV_EVENT_CLICKED, NULL);

    lv_obj_t * sh_btn_1 = lv_button_create(sh_scroll_cont);
    lv_obj_set_name(sh_btn_1, "sh_btn_1");
    lv_obj_set_x(sh_btn_1, 410);
    lv_obj_set_y(sh_btn_1, 0);
    lv_obj_set_width(sh_btn_1, 370);
    lv_obj_set_height(sh_btn_1, 120);
    lv_obj_set_style_bg_color(sh_btn_1, lv_color_hex(0x3a5066), 0);
    lv_obj_set_style_bg_opa(sh_btn_1, 255, 0);
    lv_obj_set_style_radius(sh_btn_1, 12, 0);
    lv_obj_set_style_border_width(sh_btn_1, 0, 0);
    lv_obj_set_style_layout(sh_btn_1, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(sh_btn_1, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(sh_btn_1, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(sh_btn_1, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_t * sh_lbl_1 = lv_label_create(sh_btn_1);
    lv_obj_set_name(sh_lbl_1, "sh_lbl_1");
    lv_obj_set_width(sh_lbl_1, LV_SIZE_CONTENT);
    lv_obj_set_height(sh_lbl_1, LV_SIZE_CONTENT);
    lv_label_set_text(sh_lbl_1, "IO2");
    lv_obj_set_style_text_color(sh_lbl_1, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(sh_lbl_1, font_chinese_6500_14, 0);

    lv_obj_add_event_cb(sh_btn_1, action_sh_btn_click, LV_EVENT_CLICKED, NULL);

    lv_obj_t * sh_btn_2 = lv_button_create(sh_scroll_cont);
    lv_obj_set_name(sh_btn_2, "sh_btn_2");
    lv_obj_set_x(sh_btn_2, 20);
    lv_obj_set_y(sh_btn_2, 132);
    lv_obj_set_width(sh_btn_2, 370);
    lv_obj_set_height(sh_btn_2, 120);
    lv_obj_set_style_bg_color(sh_btn_2, lv_color_hex(0x3a5066), 0);
    lv_obj_set_style_bg_opa(sh_btn_2, 255, 0);
    lv_obj_set_style_radius(sh_btn_2, 12, 0);
    lv_obj_set_style_border_width(sh_btn_2, 0, 0);
    lv_obj_set_style_layout(sh_btn_2, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(sh_btn_2, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(sh_btn_2, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(sh_btn_2, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_t * sh_lbl_2 = lv_label_create(sh_btn_2);
    lv_obj_set_name(sh_lbl_2, "sh_lbl_2");
    lv_obj_set_width(sh_lbl_2, LV_SIZE_CONTENT);
    lv_obj_set_height(sh_lbl_2, LV_SIZE_CONTENT);
    lv_label_set_text(sh_lbl_2, "IO3");
    lv_obj_set_style_text_color(sh_lbl_2, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(sh_lbl_2, font_chinese_6500_14, 0);

    lv_obj_add_event_cb(sh_btn_2, action_sh_btn_click, LV_EVENT_CLICKED, NULL);

    lv_obj_t * sh_btn_3 = lv_button_create(sh_scroll_cont);
    lv_obj_set_name(sh_btn_3, "sh_btn_3");
    lv_obj_set_x(sh_btn_3, 410);
    lv_obj_set_y(sh_btn_3, 132);
    lv_obj_set_width(sh_btn_3, 370);
    lv_obj_set_height(sh_btn_3, 120);
    lv_obj_set_style_bg_color(sh_btn_3, lv_color_hex(0x3a5066), 0);
    lv_obj_set_style_bg_opa(sh_btn_3, 255, 0);
    lv_obj_set_style_radius(sh_btn_3, 12, 0);
    lv_obj_set_style_border_width(sh_btn_3, 0, 0);
    lv_obj_set_style_layout(sh_btn_3, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(sh_btn_3, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(sh_btn_3, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(sh_btn_3, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_t * sh_lbl_3 = lv_label_create(sh_btn_3);
    lv_obj_set_name(sh_lbl_3, "sh_lbl_3");
    lv_obj_set_width(sh_lbl_3, LV_SIZE_CONTENT);
    lv_obj_set_height(sh_lbl_3, LV_SIZE_CONTENT);
    lv_label_set_text(sh_lbl_3, "IO4");
    lv_obj_set_style_text_color(sh_lbl_3, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(sh_lbl_3, font_chinese_6500_14, 0);

    lv_obj_add_event_cb(sh_btn_3, action_sh_btn_click, LV_EVENT_CLICKED, NULL);

    lv_obj_t * sh_btn_4 = lv_button_create(sh_scroll_cont);
    lv_obj_set_name(sh_btn_4, "sh_btn_4");
    lv_obj_set_x(sh_btn_4, 20);
    lv_obj_set_y(sh_btn_4, 264);
    lv_obj_set_width(sh_btn_4, 370);
    lv_obj_set_height(sh_btn_4, 120);
    lv_obj_set_style_bg_color(sh_btn_4, lv_color_hex(0x3a5066), 0);
    lv_obj_set_style_bg_opa(sh_btn_4, 255, 0);
    lv_obj_set_style_radius(sh_btn_4, 12, 0);
    lv_obj_set_style_border_width(sh_btn_4, 0, 0);
    lv_obj_set_style_layout(sh_btn_4, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(sh_btn_4, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(sh_btn_4, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(sh_btn_4, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_t * sh_lbl_4 = lv_label_create(sh_btn_4);
    lv_obj_set_name(sh_lbl_4, "sh_lbl_4");
    lv_obj_set_width(sh_lbl_4, LV_SIZE_CONTENT);
    lv_obj_set_height(sh_lbl_4, LV_SIZE_CONTENT);
    lv_label_set_text(sh_lbl_4, "IO5");
    lv_obj_set_style_text_color(sh_lbl_4, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(sh_lbl_4, font_chinese_6500_14, 0);

    lv_obj_add_event_cb(sh_btn_4, action_sh_btn_click, LV_EVENT_CLICKED, NULL);

    lv_obj_t * sh_btn_5 = lv_button_create(sh_scroll_cont);
    lv_obj_set_name(sh_btn_5, "sh_btn_5");
    lv_obj_set_x(sh_btn_5, 410);
    lv_obj_set_y(sh_btn_5, 264);
    lv_obj_set_width(sh_btn_5, 370);
    lv_obj_set_height(sh_btn_5, 120);
    lv_obj_set_style_bg_color(sh_btn_5, lv_color_hex(0x3a5066), 0);
    lv_obj_set_style_bg_opa(sh_btn_5, 255, 0);
    lv_obj_set_style_radius(sh_btn_5, 12, 0);
    lv_obj_set_style_border_width(sh_btn_5, 0, 0);
    lv_obj_set_style_layout(sh_btn_5, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(sh_btn_5, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(sh_btn_5, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(sh_btn_5, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_t * sh_lbl_5 = lv_label_create(sh_btn_5);
    lv_obj_set_name(sh_lbl_5, "sh_lbl_5");
    lv_obj_set_width(sh_lbl_5, LV_SIZE_CONTENT);
    lv_obj_set_height(sh_lbl_5, LV_SIZE_CONTENT);
    lv_label_set_text(sh_lbl_5, "IO6");
    lv_obj_set_style_text_color(sh_lbl_5, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(sh_lbl_5, font_chinese_6500_14, 0);

    lv_obj_add_event_cb(sh_btn_5, action_sh_btn_click, LV_EVENT_CLICKED, NULL);

    lv_obj_t * sh_btn_6 = lv_button_create(sh_scroll_cont);
    lv_obj_set_name(sh_btn_6, "sh_btn_6");
    lv_obj_set_x(sh_btn_6, 20);
    lv_obj_set_y(sh_btn_6, 396);
    lv_obj_set_width(sh_btn_6, 370);
    lv_obj_set_height(sh_btn_6, 120);
    lv_obj_set_style_bg_color(sh_btn_6, lv_color_hex(0x3a5066), 0);
    lv_obj_set_style_bg_opa(sh_btn_6, 255, 0);
    lv_obj_set_style_radius(sh_btn_6, 12, 0);
    lv_obj_set_style_border_width(sh_btn_6, 0, 0);
    lv_obj_set_style_layout(sh_btn_6, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(sh_btn_6, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(sh_btn_6, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(sh_btn_6, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_t * sh_lbl_6 = lv_label_create(sh_btn_6);
    lv_obj_set_name(sh_lbl_6, "sh_lbl_6");
    lv_obj_set_width(sh_lbl_6, LV_SIZE_CONTENT);
    lv_obj_set_height(sh_lbl_6, LV_SIZE_CONTENT);
    lv_label_set_text(sh_lbl_6, "IO7");
    lv_obj_set_style_text_color(sh_lbl_6, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(sh_lbl_6, font_chinese_6500_14, 0);

    lv_obj_add_event_cb(sh_btn_6, action_sh_btn_click, LV_EVENT_CLICKED, NULL);

    wifi_btn_create(lv_obj_0);

    volume_slider_create(lv_obj_0);

    nav_bar_create(lv_obj_0);

    lv_obj_t * sh_reset_btn = lv_button_create(lv_obj_0);
    lv_obj_set_name(sh_reset_btn, "sh_reset_btn");
    lv_obj_set_x(sh_reset_btn, 365);
    lv_obj_set_y(sh_reset_btn, 430);
    lv_obj_set_width(sh_reset_btn, 70);
    lv_obj_set_height(sh_reset_btn, 40);
    lv_obj_add_style(sh_reset_btn, &style_user_btn, 0);
    lv_obj_t * sh_reset_lbl = lv_label_create(sh_reset_btn);
    lv_obj_set_name(sh_reset_lbl, "sh_reset_lbl");
    lv_obj_set_width(sh_reset_lbl, LV_SIZE_CONTENT);
    lv_obj_set_height(sh_reset_lbl, LV_SIZE_CONTENT);
    lv_label_set_text(sh_reset_lbl, "恢复默认");
    lv_obj_set_style_text_color(sh_reset_lbl, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(sh_reset_lbl, font_chinese_6500_14, 0);
    lv_obj_add_style(sh_reset_lbl, &style_user_icon_label, 0);

    lv_obj_add_event_cb(sh_reset_btn, action_sh_reset, LV_EVENT_CLICKED, NULL);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

