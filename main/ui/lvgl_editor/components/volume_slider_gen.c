/**
 * @file volume_slider_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "volume_slider_gen.h"
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

lv_obj_t * volume_slider_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * lv_obj_0 = lv_obj_create(parent);
    lv_obj_set_name_static(lv_obj_0, "volume_slider_#");
    lv_obj_set_width(lv_obj_0, 800);
    lv_obj_set_height(lv_obj_0, 480);
    lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_CLICKABLE, false);
    lv_obj_set_style_bg_opa(lv_obj_0, 0, 0);
    lv_obj_set_style_border_width(lv_obj_0, 0, 0);
    lv_obj_set_style_pad_left(lv_obj_0, 0, 0);
    lv_obj_set_style_pad_top(lv_obj_0, 0, 0);
    lv_obj_set_style_pad_right(lv_obj_0, 0, 0);
    lv_obj_set_style_pad_bottom(lv_obj_0, 0, 0);

    lv_obj_t * volume_panel = lv_obj_create(lv_obj_0);
    lv_obj_set_name(volume_panel, "volume_panel");
    lv_obj_set_x(volume_panel, 200);
    lv_obj_set_y(volume_panel, -480);
    lv_obj_set_width(volume_panel, 400);
    lv_obj_set_height(volume_panel, 180);
    lv_obj_set_flag(volume_panel, LV_OBJ_FLAG_HIDDEN, true);
    lv_obj_set_flag(volume_panel, LV_OBJ_FLAG_CLICKABLE, false);
    lv_obj_set_style_radius(volume_panel, 12, 0);
    lv_obj_set_style_bg_color(volume_panel, lv_color_hex(0x1a2c42), 0);
    lv_obj_set_style_bg_opa(volume_panel, 255, 0);
    lv_obj_set_style_border_width(volume_panel, 1, 0);
    lv_obj_set_style_border_color(volume_panel, lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_layout(volume_panel, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(volume_panel, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(volume_panel, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(volume_panel, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_track_place(volume_panel, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_left(volume_panel, 0, 0);
    lv_obj_set_style_pad_top(volume_panel, 16, 0);
    lv_obj_set_style_pad_right(volume_panel, 0, 0);
    lv_obj_set_style_pad_bottom(volume_panel, 16, 0);
    lv_obj_set_style_pad_row(volume_panel, 16, 0);
    lv_obj_t * volume_title = lv_label_create(volume_panel);
    lv_obj_set_name(volume_title, "volume_title");
    lv_obj_set_width(volume_title, LV_SIZE_CONTENT);
    lv_obj_set_height(volume_title, LV_SIZE_CONTENT);
    lv_label_set_text(volume_title, "音量调节");
    lv_obj_set_style_text_color(volume_title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(volume_title, font_chinese_6500_14, 0);

    lv_obj_t * volume_slider = lv_slider_create(volume_panel);
    lv_obj_set_name(volume_slider, "volume_slider");
    lv_obj_set_width(volume_slider, 320);
    lv_obj_set_height(volume_slider, 22);
    lv_slider_set_value(volume_slider, 25, false);
    lv_obj_set_style_bg_color(volume_slider, lv_color_hex(0x2a4055), LV_PART_INDICATOR | LV_PART_KNOB);
    lv_obj_set_style_outline_color(volume_slider, lv_color_hex(0xffffff), LV_PART_KNOB);
    lv_obj_set_style_outline_width(volume_slider, 2, LV_PART_KNOB);
    lv_obj_set_style_pad_top(volume_slider, 3, LV_PART_KNOB);
    lv_obj_set_style_pad_bottom(volume_slider, 3, LV_PART_KNOB);
    lv_obj_set_style_pad_left(volume_slider, 3, LV_PART_KNOB);
    lv_obj_set_style_pad_right(volume_slider, 3, LV_PART_KNOB);
    lv_obj_add_style(volume_slider, &style_volume_slider_main, 0);
    lv_obj_add_style(volume_slider, &style_volume_slider_knob, LV_PART_KNOB);
    lv_obj_add_event_cb(volume_slider, action_volume_change, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t * volume_canel = lv_button_create(volume_panel);
    lv_obj_set_name(volume_canel, "volume_canel");
    lv_obj_set_width(volume_canel, 80);
    lv_obj_set_height(volume_canel, 36);
    lv_obj_add_style(volume_canel, &style_user_btn, 0);
    lv_obj_t * volume_canel_icon = lv_label_create(volume_canel);
    lv_obj_set_name(volume_canel_icon, "volume_canel_icon");
    lv_obj_set_width(volume_canel_icon, LV_SIZE_CONTENT);
    lv_obj_set_height(volume_canel_icon, LV_SIZE_CONTENT);
    lv_obj_set_align(volume_canel_icon, LV_ALIGN_CENTER);
    lv_obj_add_style(volume_canel_icon, &style_user_icon_label, 0);

    lv_obj_add_event_cb(volume_canel, hide_volume_panel, LV_EVENT_CLICKED, NULL);

    lv_obj_t * volume_entry_btn = lv_button_create(lv_obj_0);
    lv_obj_set_name(volume_entry_btn, "volume_entry_btn");
    lv_obj_set_x(volume_entry_btn, 0);
    lv_obj_set_y(volume_entry_btn, 0);
    lv_obj_set_width(volume_entry_btn, 70);
    lv_obj_set_height(volume_entry_btn, 40);
    lv_obj_add_style(volume_entry_btn, &style_user_btn, 0);
    lv_obj_t * volume_icon = lv_label_create(volume_entry_btn);
    lv_obj_set_name(volume_icon, "volume_icon");
    lv_obj_set_width(volume_icon, LV_SIZE_CONTENT);
    lv_obj_set_height(volume_icon, LV_SIZE_CONTENT);
    lv_obj_set_align(volume_icon, LV_ALIGN_CENTER);
    lv_obj_add_style(volume_icon, &style_user_icon_label, 0);

    lv_obj_add_event_cb(volume_entry_btn, show_volume_panel, LV_EVENT_CLICKED, NULL);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

