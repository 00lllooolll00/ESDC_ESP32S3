#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: user_btn
//

void init_style_user_btn_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_border_color(style, lv_color_hex(0xffffff));
    lv_style_set_shadow_color(style, lv_color_hex(0x000000));
    lv_style_set_shadow_ofs_y(style, 5);
    lv_style_set_shadow_ofs_x(style, 2);
    lv_style_set_shadow_width(style, 10);
    lv_style_set_bg_color(style, lv_color_hex(0x7c848a));
    lv_style_set_layout(style, LV_LAYOUT_FLEX);
    lv_style_set_flex_main_place(style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_cross_place(style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(style, LV_FLEX_ALIGN_CENTER);
};

lv_style_t *get_style_user_btn_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_user_btn_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_user_btn(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_user_btn_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_user_btn(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_user_btn_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: wifi_loadder
//

void add_style_wifi_loadder(lv_obj_t *obj) {
    (void)obj;
};

void remove_style_wifi_loadder(lv_obj_t *obj) {
    (void)obj;
};

//
// Style: user_icon_label
//

void init_style_user_icon_label_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0x252424));
    lv_style_set_text_font(style, &lv_font_montserrat_24);
};

lv_style_t *get_style_user_icon_label_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_user_icon_label_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_user_icon_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_user_icon_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_user_icon_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_user_icon_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: user_text_label
//

void init_style_user_text_label_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0x212121));
    lv_style_set_text_font(style, ui_font_chinese_3500_14);
};

lv_style_t *get_style_user_text_label_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_user_text_label_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_user_text_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_user_text_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_user_text_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_user_text_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: volume_slider
//

void init_style_volume_slider_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0x212121));
    lv_style_set_text_font(style, &lv_font_montserrat_16);
    lv_style_set_border_color(style, lv_color_hex(0xffffff));
    lv_style_set_shadow_width(style, 25);
    lv_style_set_shadow_ofs_x(style, 2);
    lv_style_set_shadow_ofs_y(style, 5);
    lv_style_set_shadow_color(style, lv_color_hex(0x000000));
    lv_style_set_line_color(style, lv_color_hex(0x212121));
};

lv_style_t *get_style_volume_slider_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_volume_slider_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_volume_slider_KNOB_DEFAULT(lv_style_t *style) {
    lv_style_set_line_color(style, lv_color_lighten(lv_color_hex(0x000000), 0));
};

lv_style_t *get_style_volume_slider_KNOB_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_volume_slider_KNOB_DEFAULT(style);
    }
    return style;
};

void add_style_volume_slider(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_volume_slider_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_volume_slider_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

void remove_style_volume_slider(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_volume_slider_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_volume_slider_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_user_btn,
        add_style_wifi_loadder,
        add_style_user_icon_label,
        add_style_user_text_label,
        add_style_volume_slider,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_user_btn,
        remove_style_wifi_loadder,
        remove_style_user_icon_label,
        remove_style_user_text_label,
        remove_style_volume_slider,
    };
    remove_style_funcs[styleIndex](obj);
}