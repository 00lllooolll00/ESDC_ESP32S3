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

    lv_obj_t * smart_home_title = lv_label_create(lv_obj_0);
    lv_obj_set_name(smart_home_title, "smart_home_title");
    lv_obj_set_x(smart_home_title, 300);
    lv_obj_set_y(smart_home_title, 220);
    lv_obj_set_width(smart_home_title, LV_SIZE_CONTENT);
    lv_obj_set_height(smart_home_title, LV_SIZE_CONTENT);
    lv_label_set_text(smart_home_title, "智能家具");
    lv_obj_set_style_text_color(smart_home_title, lv_color_hex(0x212121), 0);

    wifi_btn_create(lv_obj_0);

    volume_slider_create(lv_obj_0);

    nav_bar_create(lv_obj_0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

