/**
 * @file wifi_btn_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "wifi_btn_gen.h"
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

lv_obj_t * wifi_btn_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * lv_obj_0 = lv_obj_create(parent);
    lv_obj_set_name_static(lv_obj_0, "wifi_btn_#");
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

    lv_obj_t * wifi_list_panel = lv_obj_create(lv_obj_0);
    lv_obj_set_name(wifi_list_panel, "wifi_list_panel");
    lv_obj_set_x(wifi_list_panel, 150);
    lv_obj_set_y(wifi_list_panel, -480);
    lv_obj_set_width(wifi_list_panel, 500);
    lv_obj_set_height(wifi_list_panel, 480);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_HIDDEN, true);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_CLICKABLE, false);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_GESTURE_BUBBLE, false);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_PRESS_LOCK, false);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_SCROLL_CHAIN_HOR, false);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_SCROLL_CHAIN_VER, false);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_SCROLL_ELASTIC, false);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_SCROLL_MOMENTUM, false);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_SCROLL_WITH_ARROW, false);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_SNAPPABLE, false);
    lv_obj_set_flag(wifi_list_panel, LV_OBJ_FLAG_CLICK_FOCUSABLE, false);
    lv_obj_set_style_bg_color(wifi_list_panel, lv_color_hex(0xeae9e7), 0);
    lv_obj_set_style_pad_left(wifi_list_panel, 0, 0);
    lv_obj_set_style_pad_top(wifi_list_panel, 0, 0);
    lv_obj_set_style_pad_right(wifi_list_panel, 0, 0);
    lv_obj_set_style_pad_bottom(wifi_list_panel, 0, 0);
    lv_obj_set_style_border_width(wifi_list_panel, 0, 0);
    lv_obj_t * wifi_refresh_btn = lv_button_create(wifi_list_panel);
    lv_obj_set_name(wifi_refresh_btn, "wifi_refresh_btn");
    lv_obj_set_x(wifi_refresh_btn, 11);
    lv_obj_set_y(wifi_refresh_btn, 389);
    lv_obj_set_width(wifi_refresh_btn, 70);
    lv_obj_set_height(wifi_refresh_btn, 40);
    lv_obj_set_style_layout(wifi_refresh_btn, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_main_place(wifi_refresh_btn, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(wifi_refresh_btn, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_track_place(wifi_refresh_btn, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_add_style(wifi_refresh_btn, &style_user_btn, 0);
    lv_obj_t * wifi_refresh_icon = lv_label_create(wifi_refresh_btn);
    lv_obj_set_name(wifi_refresh_icon, "wifi_refresh_icon");
    lv_obj_set_width(wifi_refresh_icon, LV_SIZE_CONTENT);
    lv_obj_set_height(wifi_refresh_icon, LV_SIZE_CONTENT);
    lv_obj_add_style(wifi_refresh_icon, &style_user_icon_label, 0);

    lv_obj_add_event_cb(wifi_refresh_btn, action_wifi_start_scan, LV_EVENT_CLICKED, NULL);

    lv_obj_t * wifi_cancel_btn = lv_button_create(wifi_list_panel);
    lv_obj_set_name(wifi_cancel_btn, "wifi_cancel_btn");
    lv_obj_set_x(wifi_cancel_btn, 376);
    lv_obj_set_y(wifi_cancel_btn, 389);
    lv_obj_set_width(wifi_cancel_btn, 70);
    lv_obj_set_height(wifi_cancel_btn, 40);
    lv_obj_add_style(wifi_cancel_btn, &style_user_btn, 0);
    lv_obj_t * wifi_cancel_icon = lv_label_create(wifi_cancel_btn);
    lv_obj_set_name(wifi_cancel_icon, "wifi_cancel_icon");
    lv_obj_set_width(wifi_cancel_icon, LV_SIZE_CONTENT);
    lv_obj_set_height(wifi_cancel_icon, LV_SIZE_CONTENT);
    lv_obj_add_style(wifi_cancel_icon, &style_user_icon_label, 0);

    lv_obj_add_event_cb(wifi_cancel_btn, hide_wifi_panel, LV_EVENT_CLICKED, NULL);

    lv_obj_t * wifi_list = lv_obj_create(wifi_list_panel);
    lv_obj_set_name(wifi_list, "wifi_list");
    lv_obj_set_x(wifi_list, -22);
    lv_obj_set_y(wifi_list, -22);
    lv_obj_set_width(wifi_list, 500);
    lv_obj_set_height(wifi_list, 392);
    lv_obj_set_style_bg_color(wifi_list, lv_color_hex(0xd5d3d3), 0);
    lv_obj_set_style_layout(wifi_list, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_main_place(wifi_list, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_track_place(wifi_list, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(wifi_list, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_t * wifi_loader = lv_spinner_create(wifi_list);
    lv_obj_set_name(wifi_loader, "wifi_loader");
    lv_obj_set_width(wifi_loader, 80);
    lv_obj_set_height(wifi_loader, 80);
    lv_obj_set_style_arc_color(wifi_loader, lv_color_hex(0x69727a), LV_PART_INDICATOR);

    lv_obj_t * wifi_btn = lv_button_create(lv_obj_0);
    lv_obj_set_name(wifi_btn, "wifi_btn");
    lv_obj_set_x(wifi_btn, 730);
    lv_obj_set_y(wifi_btn, 0);
    lv_obj_set_width(wifi_btn, 70);
    lv_obj_set_height(wifi_btn, 40);
    lv_obj_add_style(wifi_btn, &style_user_btn, 0);
    lv_obj_t * wifi_icon = lv_label_create(wifi_btn);
    lv_obj_set_name(wifi_icon, "wifi_icon");
    lv_obj_set_width(wifi_icon, LV_SIZE_CONTENT);
    lv_obj_set_height(wifi_icon, LV_SIZE_CONTENT);
    lv_obj_add_style(wifi_icon, &style_user_icon_label, 0);

    lv_obj_t * wifi_state = lv_label_create(wifi_btn);
    lv_obj_set_name(wifi_state, "wifi_state");
    lv_obj_set_width(wifi_state, LV_SIZE_CONTENT);
    lv_obj_set_height(wifi_state, LV_SIZE_CONTENT);
    lv_obj_add_style(wifi_state, &style_user_icon_label, 0);

    lv_obj_add_event_cb(wifi_btn, action_wifi_start_scan, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(wifi_btn, show_wifi_panel, LV_EVENT_CLICKED, NULL);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

