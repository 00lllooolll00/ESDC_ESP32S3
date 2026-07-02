/**
 * @file nav_bar_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "nav_bar_gen.h"
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

lv_obj_t * nav_bar_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * lv_obj_0 = lv_obj_create(parent);
    lv_obj_set_name_static(lv_obj_0, "nav_bar_#");
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

    lv_obj_t * nav_home_btn = lv_button_create(lv_obj_0);
    lv_obj_set_name(nav_home_btn, "nav_home_btn");
    lv_obj_set_x(nav_home_btn, 10);
    lv_obj_set_y(nav_home_btn, 430);
    lv_obj_set_width(nav_home_btn, 70);
    lv_obj_set_height(nav_home_btn, 40);
    lv_obj_add_style(nav_home_btn, &style_user_btn, 0);
    lv_obj_t * nav_home_icon = lv_label_create(nav_home_btn);
    lv_obj_set_name(nav_home_icon, "nav_home_icon");
    lv_obj_set_width(nav_home_icon, LV_SIZE_CONTENT);
    lv_obj_set_height(nav_home_icon, LV_SIZE_CONTENT);
    lv_obj_add_style(nav_home_icon, &style_user_icon_label, 0);

    lv_obj_add_screen_load_event(nav_home_btn, LV_EVENT_CLICKED, main_page, LV_SCREEN_LOAD_ANIM_FADE_ON, 500, 0);

    lv_obj_t * nav_back_btn = lv_button_create(lv_obj_0);
    lv_obj_set_name(nav_back_btn, "nav_back_btn");
    lv_obj_set_x(nav_back_btn, 720);
    lv_obj_set_y(nav_back_btn, 430);
    lv_obj_set_width(nav_back_btn, 70);
    lv_obj_set_height(nav_back_btn, 40);
    lv_obj_add_style(nav_back_btn, &style_user_btn, 0);
    lv_obj_t * nav_back_icon = lv_label_create(nav_back_btn);
    lv_obj_set_name(nav_back_icon, "nav_back_icon");
    lv_obj_set_width(nav_back_icon, LV_SIZE_CONTENT);
    lv_obj_set_height(nav_back_icon, LV_SIZE_CONTENT);
    lv_obj_add_style(nav_back_icon, &style_user_icon_label, 0);

    lv_obj_add_event_cb(nav_back_btn, nav_back, LV_EVENT_CLICKED, NULL);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

