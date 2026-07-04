/**
 * @file ai_chat_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "ai_chat_gen.h"
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

lv_obj_t * ai_chat_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    static bool style_inited = false;

    if (!style_inited) {

        style_inited = true;
    }

    if (ai_chat == NULL) ai_chat = lv_obj_create(NULL);
    lv_obj_t * lv_obj_0 = ai_chat;
    lv_obj_set_name_static(lv_obj_0, "ai_chat_#");
    lv_obj_set_style_bg_color(lv_obj_0, lv_color_hex(0x1a2c42), 0);
    lv_obj_set_style_bg_opa(lv_obj_0, 255, 0);

    lv_obj_t * ai_chat_title = lv_label_create(lv_obj_0);
    lv_obj_set_name(ai_chat_title, "ai_chat_title");
    lv_obj_set_x(ai_chat_title, 300);
    lv_obj_set_y(ai_chat_title, 220);
    lv_obj_set_width(ai_chat_title, LV_SIZE_CONTENT);
    lv_obj_set_height(ai_chat_title, LV_SIZE_CONTENT);
    lv_label_set_text(ai_chat_title, "AI对话");
    lv_obj_set_style_text_color(ai_chat_title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(ai_chat_title, font_chinese_3500_18, 0);

    wifi_btn_create(lv_obj_0);

    volume_slider_create(lv_obj_0);

    nav_bar_create(lv_obj_0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

