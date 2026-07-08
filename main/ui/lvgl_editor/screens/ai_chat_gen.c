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
    lv_obj_set_x(ai_chat_title, 340);
    lv_obj_set_y(ai_chat_title, 6);
    lv_obj_set_width(ai_chat_title, 120);
    lv_obj_set_height(ai_chat_title, 28);
    lv_label_set_text(ai_chat_title, "AI对话");
    lv_obj_set_style_text_color(ai_chat_title, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(ai_chat_title, font_chinese_3500_18, 0);
    lv_obj_set_style_text_align(ai_chat_title, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_t * ai_chat_msg_list = lv_obj_create(lv_obj_0);
    lv_obj_set_name(ai_chat_msg_list, "ai_chat_msg_list");
    lv_obj_set_x(ai_chat_msg_list, 10);
    lv_obj_set_y(ai_chat_msg_list, 38);
    lv_obj_set_width(ai_chat_msg_list, 780);
    lv_obj_set_height(ai_chat_msg_list, 380);
    lv_obj_set_flag(ai_chat_msg_list, LV_OBJ_FLAG_SCROLLABLE, true);
    lv_obj_set_style_bg_opa(ai_chat_msg_list, 0, 0);
    lv_obj_set_style_border_width(ai_chat_msg_list, 0, 0);
    lv_obj_set_style_pad_left(ai_chat_msg_list, 8, 0);
    lv_obj_set_style_pad_right(ai_chat_msg_list, 8, 0);
    lv_obj_set_style_pad_top(ai_chat_msg_list, 4, 0);
    lv_obj_set_style_pad_bottom(ai_chat_msg_list, 4, 0);
    lv_obj_set_style_layout(ai_chat_msg_list, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(ai_chat_msg_list, LV_FLEX_FLOW_COLUMN, 0);

    lv_obj_t * ai_chat_loading = lv_label_create(lv_obj_0);
    lv_obj_set_name(ai_chat_loading, "ai_chat_loading");
    lv_obj_set_x(ai_chat_loading, 340);
    lv_obj_set_y(ai_chat_loading, 200);
    lv_obj_set_width(ai_chat_loading, 120);
    lv_obj_set_height(ai_chat_loading, 20);
    lv_label_set_text(ai_chat_loading, "AI正在回复...");
    lv_obj_set_flag(ai_chat_loading, LV_OBJ_FLAG_HIDDEN, true);
    lv_obj_set_style_text_color(ai_chat_loading, lv_color_hex(0xa0b0c0), 0);
    lv_obj_set_style_text_font(ai_chat_loading, font_chinese_6500_14, 0);
    lv_obj_set_style_text_align(ai_chat_loading, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_t * ai_chat_voice_cont = lv_obj_create(lv_obj_0);
    lv_obj_set_name(ai_chat_voice_cont, "ai_chat_voice_cont");
    lv_obj_set_x(ai_chat_voice_cont, 0);
    lv_obj_set_y(ai_chat_voice_cont, 430);
    lv_obj_set_width(ai_chat_voice_cont, 800);
    lv_obj_set_height(ai_chat_voice_cont, 40);
    lv_obj_set_flag(ai_chat_voice_cont, LV_OBJ_FLAG_SCROLLABLE, false);
    lv_obj_set_style_bg_opa(ai_chat_voice_cont, 0, 0);
    lv_obj_set_style_border_width(ai_chat_voice_cont, 0, 0);
    lv_obj_set_style_pad_all(ai_chat_voice_cont, 0, 0);
    lv_obj_set_style_layout(ai_chat_voice_cont, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(ai_chat_voice_cont, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(ai_chat_voice_cont, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(ai_chat_voice_cont, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_t * ai_chat_voice_btn = lv_button_create(ai_chat_voice_cont);
    lv_obj_set_name(ai_chat_voice_btn, "ai_chat_voice_btn");
    lv_obj_set_width(ai_chat_voice_btn, 600);
    lv_obj_set_height(ai_chat_voice_btn, 40);
    lv_obj_add_style(ai_chat_voice_btn, &style_user_btn, 0);
    lv_obj_t * ai_chat_voice_label = lv_label_create(ai_chat_voice_btn);
    lv_obj_set_name(ai_chat_voice_label, "ai_chat_voice_label");
    lv_obj_set_width(ai_chat_voice_label, LV_SIZE_CONTENT);
    lv_obj_set_height(ai_chat_voice_label, LV_SIZE_CONTENT);
    lv_label_set_text(ai_chat_voice_label, "按住说话");
    lv_obj_set_style_text_color(ai_chat_voice_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(ai_chat_voice_label, font_chinese_6500_14, 0);

    wifi_btn_create(lv_obj_0);

    volume_slider_create(lv_obj_0);

    nav_bar_create(lv_obj_0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

