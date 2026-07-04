/**
 * @file ui_gen.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_gen.h"

#if LV_USE_XML
#endif /* LV_USE_XML */

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/*----------------
 * Translations
 *----------------*/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/*--------------------
 *  Permanent screens
 *-------------------*/

lv_obj_t * ai_chat = NULL;
lv_obj_t * main_page = NULL;
lv_obj_t * smart_home = NULL;
lv_obj_t * weather = NULL;

/*----------------
 * Fonts
 *----------------*/

lv_font_t * font_chinese_6500_14;

/*----------------
 * Images
 *----------------*/

/*----------------
 * Global styles
 *----------------*/

lv_style_t style_user_btn;
lv_style_t style_user_icon_label;
lv_style_t style_user_text_label;
lv_style_t style_volume_slider_main;
lv_style_t style_volume_slider_knob;
lv_style_t style_func_card;
lv_style_t style_weather_card;

/*----------------
 * Subjects
 *----------------*/

lv_subject_t wifi_is_connected;
lv_subject_t volume;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ui_init_gen(const char * asset_path)
{
    char buf[256];


    /*----------------
     * Fonts
     *----------------*/

    /* create bin font 'font_chinese_6500_14' from file */
    lv_snprintf(buf, 256, "%s%s", asset_path, "fonts/font_chinese_6500_14.bin");
    font_chinese_6500_14 = lv_binfont_create(buf);


    /*----------------
     * Images
     *----------------*/
    /*----------------
     * Global styles
     *----------------*/

    static bool style_inited = false;

    if (!style_inited) {
        lv_style_init(&style_user_btn);
        lv_style_set_bg_color(&style_user_btn, lv_color_hex(0x3a5066));
        lv_style_set_layout(&style_user_btn, LV_LAYOUT_FLEX);
        lv_style_set_flex_main_place(&style_user_btn, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_cross_place(&style_user_btn, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_track_place(&style_user_btn, LV_FLEX_ALIGN_CENTER);

        lv_style_init(&style_user_icon_label);
        lv_style_set_text_color(&style_user_icon_label, lv_color_hex(0xffffff));

        lv_style_init(&style_user_text_label);
        lv_style_set_text_color(&style_user_text_label, lv_color_hex(0xffffff));

        lv_style_init(&style_volume_slider_main);
        lv_style_set_text_color(&style_volume_slider_main, lv_color_hex(0xa0b0c0));
        lv_style_set_border_color(&style_volume_slider_main, lv_color_hex(0x3a5066));

        lv_style_init(&style_volume_slider_knob);
        lv_style_set_line_color(&style_volume_slider_knob, lv_color_hex(0xffffff));

        lv_style_init(&style_func_card);
        lv_style_set_bg_color(&style_func_card, lv_color_hex(0x2a4055));
        lv_style_set_layout(&style_func_card, LV_LAYOUT_FLEX);
        lv_style_set_flex_main_place(&style_func_card, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_cross_place(&style_func_card, LV_FLEX_ALIGN_CENTER);
        lv_style_set_flex_track_place(&style_func_card, LV_FLEX_ALIGN_CENTER);
        lv_style_set_radius(&style_func_card, 12);
        lv_style_set_border_width(&style_func_card, 0);

        lv_style_init(&style_weather_card);
        lv_style_set_bg_color(&style_weather_card, lv_color_hex(0x2a4055));
        lv_style_set_bg_opa(&style_weather_card, 255);
        lv_style_set_radius(&style_weather_card, 8);
        lv_style_set_border_width(&style_weather_card, 0);

        style_inited = true;
    }

    /*----------------
     * Subjects
     *----------------*/
    lv_subject_init_int(&wifi_is_connected, 0);
    lv_subject_init_int(&volume, 25);

    /*----------------
     * Translations
     *----------------*/

#if LV_USE_XML
    /* Register widgets */

    /* Register fonts */
    lv_xml_register_font(NULL, "font_chinese_6500_14", font_chinese_6500_14);

    /* Register subjects */
    lv_xml_register_subject(NULL, "wifi_is_connected", &wifi_is_connected);
    lv_xml_register_subject(NULL, "volume", &volume);

    /* Register callbacks */
    lv_xml_register_event_cb(NULL, "nav_back", nav_back);
    lv_xml_register_event_cb(NULL, "action_volume_change", action_volume_change);
    lv_xml_register_event_cb(NULL, "hide_volume_panel", hide_volume_panel);
    lv_xml_register_event_cb(NULL, "show_volume_panel", show_volume_panel);
    lv_xml_register_event_cb(NULL, "action_wifi_start_scan", action_wifi_start_scan);
    lv_xml_register_event_cb(NULL, "hide_wifi_panel", hide_wifi_panel);
    lv_xml_register_event_cb(NULL, "show_wifi_panel", show_wifi_panel);
    lv_xml_register_event_cb(NULL, "action_sh_btn_click", action_sh_btn_click);
    lv_xml_register_event_cb(NULL, "action_sh_reset", action_sh_reset);
    lv_xml_register_event_cb(NULL, "action_weather_refresh", action_weather_refresh);
#endif

    /* Register all the global assets so that they won't be created again when globals.xml is parsed.
     * While running in the editor skip this step to update the preview when the XML changes */
#if LV_USE_XML && !defined(LV_EDITOR_PREVIEW)
    /* Register images */
#endif

#if LV_USE_XML == 0
    /*--------------------
     *  Permanent screens
     *-------------------*/
    /* If XML is enabled it's assumed that the permanent screens are created
     * manaully from XML using lv_xml_create() */
    /* To allow screens to reference each other, create them all before calling the sceen create functions */
    ai_chat = lv_obj_create(NULL);
    main_page = lv_obj_create(NULL);
    smart_home = lv_obj_create(NULL);
    weather = lv_obj_create(NULL);

    ai_chat_create();
    main_page_create();
    smart_home_create();
    weather_create();
#endif
}

/* Callbacks */
#if defined(LV_EDITOR_PREVIEW)
void __attribute__((weak)) nav_back(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("nav_back was called\n");
}
void __attribute__((weak)) action_volume_change(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("action_volume_change was called\n");
}
void __attribute__((weak)) hide_volume_panel(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("hide_volume_panel was called\n");
}
void __attribute__((weak)) show_volume_panel(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("show_volume_panel was called\n");
}
void __attribute__((weak)) action_wifi_start_scan(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("action_wifi_start_scan was called\n");
}
void __attribute__((weak)) hide_wifi_panel(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("hide_wifi_panel was called\n");
}
void __attribute__((weak)) show_wifi_panel(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("show_wifi_panel was called\n");
}
void __attribute__((weak)) action_sh_btn_click(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("action_sh_btn_click was called\n");
}
void __attribute__((weak)) action_sh_reset(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("action_sh_reset was called\n");
}
void __attribute__((weak)) action_weather_refresh(lv_event_t * e)
{
    LV_UNUSED(e);
    LV_LOG("action_weather_refresh was called\n");
}
#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/