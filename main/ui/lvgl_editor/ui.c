/**
 * @file ui.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui.h"
#include "common_header.h"
#include "ek_stack.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void _set_fonts_and_icons(lv_obj_t *screen);
static void _set_widget_font_by_name(lv_obj_t *screen, const char *name, const lv_font_t *font);
static void _set_widget_text_by_name(lv_obj_t *screen, const char *name, const char *text);
static void _on_screen_loaded_cb(lv_event_t *e);
static lv_obj_t *_nav_peek(void);

/**********************
 *  STATIC VARIABLES
 **********************/

EK_LOG_FILE_TAG("ui");


// 屏幕导航历史栈（用于 nav_back 返回上一页）
static ek_stack_t *s_nav_stack;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ui_init(const char *asset_path)
{
    // 调用生成的 init（创建 permanent screens、styles、subjects）
    ui_init_gen(asset_path);


    // 为所有 permanent screen 设置字体和图标
    _set_fonts_and_icons(main_page);
    // 天气图标：从 vfs 加载 LVGL bin 图片（不占 app 分区）
    lv_obj_t *wicon = lv_obj_find_by_name(main_page, "func_weather_icon");
    if (wicon)
    {
        lv_image_set_src(wicon, "S:/images/weather.bin");
        lv_obj_set_style_image_recolor(wicon, lv_color_white(), 0);
        lv_obj_set_style_image_recolor_opa(wicon, LV_OPA_COVER, 0);
    }
    // AI 对话图标：从 vfs 加载 LVGL bin 图片
    lv_obj_t *aicon = lv_obj_find_by_name(main_page, "func_ai_chat_icon");
    if (aicon)
    {
        lv_image_set_src(aicon, "S:/images/ai_chat.bin");
        lv_obj_set_style_image_recolor(aicon, lv_color_white(), 0);
        lv_obj_set_style_image_recolor_opa(aicon, LV_OPA_COVER, 0);
    }
    _set_fonts_and_icons(smart_home);
    _set_fonts_and_icons(weather);
    _set_fonts_and_icons(ai_chat);

    // 创建导航历史栈，初始压入 main_page
    s_nav_stack = ek_stack_create(sizeof(lv_obj_t *), 8);
    assert(s_nav_stack);
    ek_stack_push(s_nav_stack, &main_page);

    // 为每个 permanent screen 注册加载事件回调，自动维护导航历史栈
    lv_obj_add_event_cb(main_page, _on_screen_loaded_cb, LV_EVENT_SCREEN_LOAD_START, NULL);
    lv_obj_add_event_cb(smart_home, _on_screen_loaded_cb, LV_EVENT_SCREEN_LOAD_START, NULL);
    lv_obj_add_event_cb(weather, _on_screen_loaded_cb, LV_EVENT_SCREEN_LOAD_START, NULL);
    lv_obj_add_event_cb(ai_chat, _on_screen_loaded_cb, LV_EVENT_SCREEN_LOAD_START, NULL);

    // 加载主屏幕
    lv_screen_load(main_page);
}

// ============================================================================
// 屏幕导航回调
// ============================================================================

// 返回上一页
void nav_back(lv_event_t *e)
{
    lv_obj_t *current;
    // 弹出当前屏幕
    if (ek_stack_pop(s_nav_stack, &current) != EK_ERR_NONE)
        return;

    // 栈空了说明没有上一页，把当前 push 回去
    if (ek_stack_empty(s_nav_stack))
    {
        ek_stack_push(s_nav_stack, &current);
        return;
    }

    // peek 栈顶作为返回目标
    lv_obj_t *prev = _nav_peek();
    if (prev)
        lv_screen_load_anim(prev, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, false);
}

// 屏幕加载时调用（在 LV_EVENT_SCREEN_LOAD_START 中推入历史栈）
// 由 ui_init 末尾和 screen_load_event 触发时自动调用
void _on_screen_loaded(lv_obj_t *screen)
{
    // 回到主界面时清空栈，只保留 main_page
    if (screen == main_page)
    {
        s_nav_stack->sp = 0;
        ek_stack_push(s_nav_stack, &screen);
        return;
    }

    // 去重：如果栈顶已经是同一屏幕就不重复推
    if (_nav_peek() == screen)
        return;

    ek_stack_push(s_nav_stack, &screen);
}

// 屏幕加载事件回调（LV_EVENT_SCREEN_LOAD_START 触发时推入导航历史栈）
static void _on_screen_loaded_cb(lv_event_t *e)
{
    lv_obj_t *screen = lv_event_get_target_obj(e);
    _on_screen_loaded(screen);
}

// peek 栈顶元素但不弹出（pop + push 模拟）
static lv_obj_t *_nav_peek(void)
{
    lv_obj_t *top = NULL;
    if (ek_stack_pop(s_nav_stack, &top) == EK_ERR_NONE)
    {
        ek_stack_push(s_nav_stack, &top);
        return top;
    }
    return NULL;
}

// 面板滑出动画完成回调：从 anim 结构体取出 var 和 user_data 来设置 flag
static void _hide_panel_anim_ready(lv_anim_t *a)
{
    lv_obj_add_flag((lv_obj_t *)a->var, (lv_obj_flag_t)(intptr_t)a->user_data);
}

// ============================================================================
// 面板回调函数（由 XML event_cb 引用，用户实现，不会被生成器覆盖）
// ============================================================================

void show_wifi_panel(lv_event_t *e)
{
    lv_obj_t *panel = lv_obj_find_by_name(lv_screen_active(), "wifi_list_panel");
    if (panel)
    {
        lv_obj_clear_flag(panel, LV_OBJ_FLAG_HIDDEN);
        // 滑入动画：-480 → 0，400ms，EASE_IN
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, panel);
        lv_anim_set_values(&a, -480, 0);
        lv_anim_set_duration(&a, 400);
        lv_anim_set_path_cb(&a, lv_anim_path_ease_in);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
        lv_anim_start(&a);
    }
    // 隐藏 wifi 入口按钮
    lv_obj_t *btn = lv_obj_find_by_name(lv_screen_active(), "wifi_btn");
    if (btn)
    {
        lv_obj_add_flag(btn, LV_OBJ_FLAG_HIDDEN);
    }
}

void hide_wifi_panel(lv_event_t *e)
{
    lv_obj_t *panel = lv_obj_find_by_name(lv_screen_active(), "wifi_list_panel");
    if (panel)
    {
        // 滑出动画：0 → -480，300ms，EASE_OUT
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, panel);
        lv_anim_set_values(&a, 0, -480);
        lv_anim_set_duration(&a, 300);
        lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
        lv_anim_set_ready_cb(&a, _hide_panel_anim_ready);
        lv_anim_set_user_data(&a, (void *)LV_OBJ_FLAG_HIDDEN);
        lv_anim_start(&a);
    }
    // 显示 wifi 入口按钮
    lv_obj_t *btn = lv_obj_find_by_name(lv_screen_active(), "wifi_btn");
    if (btn)
    {
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_HIDDEN);
    }
}

void show_volume_panel(lv_event_t *e)
{
    lv_obj_t *panel = lv_obj_find_by_name(lv_screen_active(), "volume_panel");
    if (panel)
    {
        lv_obj_clear_flag(panel, LV_OBJ_FLAG_HIDDEN);
        // 滑入动画：-480 → 0，400ms，EASE_IN
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, panel);
        lv_anim_set_values(&a, -480, 0);
        lv_anim_set_duration(&a, 400);
        lv_anim_set_path_cb(&a, lv_anim_path_ease_in);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
        lv_anim_start(&a);
    }
}

void hide_volume_panel(lv_event_t *e)
{
    lv_obj_t *panel = lv_obj_find_by_name(lv_screen_active(), "volume_panel");
    if (panel)
    {
        // 滑出动画：0 → -480，300ms，EASE_OUT
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, panel);
        lv_anim_set_values(&a, 0, -480);
        lv_anim_set_duration(&a, 300);
        lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
        lv_anim_set_ready_cb(&a, _hide_panel_anim_ready);
        lv_anim_set_user_data(&a, (void *)LV_OBJ_FLAG_HIDDEN);
        lv_anim_start(&a);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

// 按名称查找 widget 并设置字体
static void _set_widget_font_by_name(lv_obj_t *screen, const char *name, const lv_font_t *font)
{
    lv_obj_t *obj = lv_obj_find_by_name(screen, name);
    if (obj)
    {
        lv_obj_set_style_text_font(obj, font, 0);
    }
}

// 按名称查找 widget 并设置文本
static void _set_widget_text_by_name(lv_obj_t *screen, const char *name, const char *text)
{
    lv_obj_t *obj = lv_obj_find_by_name(screen, name);
    if (obj)
    {
        lv_label_set_text(obj, text);
    }
}

// 为屏幕内所有 widget 设置字体和图标文本
static void _set_fonts_and_icons(lv_obj_t *screen)
{
    // 图标 label：montserrat_24（通用）
    _set_widget_font_by_name(screen, "wifi_refresh_icon", &lv_font_montserrat_24);
    _set_widget_font_by_name(screen, "weather_refresh_icon", &lv_font_montserrat_24);
    _set_widget_font_by_name(screen, "wifi_icon", &lv_font_montserrat_24);
    _set_widget_font_by_name(screen, "volume_icon", &lv_font_montserrat_24);
    _set_widget_font_by_name(screen, "volume_canel_icon", &lv_font_montserrat_24);
    _set_widget_font_by_name(screen, "func_smart_home_icon", &lv_font_montserrat_24);

    // 特殊字体
    _set_widget_font_by_name(screen, "wifi_cancel_icon", &lv_font_montserrat_26);
    _set_widget_font_by_name(screen, "wifi_state", &lv_font_montserrat_20);
    _set_widget_font_by_name(screen, "nav_back_icon", &lv_font_montserrat_20);
    _set_widget_font_by_name(screen, "nav_home_icon", &lv_font_montserrat_20);


    // 设置图标文本（EEZ 中通过 Flow 表达式设置，这里直接用 LVGL 符号宏）
    _set_widget_text_by_name(screen, "wifi_refresh_icon", LV_SYMBOL_REFRESH);
    _set_widget_text_by_name(screen, "weather_refresh_icon", LV_SYMBOL_REFRESH);
    _set_widget_text_by_name(screen, "wifi_cancel_icon", LV_SYMBOL_CLOSE);
    _set_widget_text_by_name(screen, "wifi_icon", LV_SYMBOL_WIFI);
    _set_widget_text_by_name(screen, "wifi_state", LV_SYMBOL_CLOSE);
    _set_widget_text_by_name(screen, "nav_back_icon", LV_SYMBOL_LEFT);
    _set_widget_text_by_name(screen, "nav_home_icon", LV_SYMBOL_HOME);
    _set_widget_text_by_name(screen, "volume_icon", LV_SYMBOL_VOLUME_MAX);
    _set_widget_text_by_name(screen, "volume_canel_icon", LV_SYMBOL_CLOSE);
    // 功能卡片图标
    _set_widget_text_by_name(screen, "func_smart_home_icon", LV_SYMBOL_HOME);
}
