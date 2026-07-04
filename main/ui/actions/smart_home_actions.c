#include "common_header.h"
#include "ui.h"
#include "app_smart_home.h"
#include "lvgl.h"

EK_LOG_FILE_TAG("smart_home_actions");

// 高电平=绿色，低电平=灰色
#define SH_COLOR_HIGH lv_color_hex(0x2ecc71)
#define SH_COLOR_LOW  lv_color_hex(0x3a5066)

// 预设中文标签：点选填入 textarea，弥补 LVGL 键盘无中文输入法
static const char *s_preset_names[] = {
    "灯光", "插座", "风扇", "窗帘", "空调", "电视", "备用"
};
#define SH_PRESET_COUNT (sizeof(s_preset_names) / sizeof(s_preset_names[0]))

// 编辑名字弹窗控件
static lv_obj_t *s_edit_cont;
static lv_obj_t *s_edit_ta;
static lv_obj_t *s_edit_kb;
static int s_edit_idx;

static void _refresh_btn(int idx);
static void _refresh_all(void);
static void _btn_long_press_cb(lv_event_t *e);
static void _edit_ready_cb(lv_event_t *e);
static void _edit_cancel_cb(lv_event_t *e);
static void _edit_cleanup(void);
static void _preset_name_cb(lv_event_t *e);

// 初始化：给 7 个按钮注册长按事件 + 存 idx 到 user_data，再刷一遍颜色/文字
void smart_home_actions_init(void)
{
    for (int i = 0; i < APP_SMART_HOME_COUNT; i++)
    {
        char namebuf[16];
        snprintf(namebuf, sizeof(namebuf), "sh_btn_%d", i);
        lv_obj_t *btn = lv_obj_find_by_name(smart_home, namebuf);
        if (!btn)
        {
            EK_LOG_WARN("smart_home_actions: btn %s not found", namebuf);
            continue;
        }
        lv_obj_set_user_data(btn, (void *)(uintptr_t)i);
        lv_obj_add_event_cb(btn, _btn_long_press_cb, LV_EVENT_LONG_PRESSED, NULL);
    }

    _refresh_all();
}

// 点击切换电平（XML event_cb 绑定 action_sh_btn_click）
void action_sh_btn_click(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_current_target(e);
    int idx = (int)(uintptr_t)lv_obj_get_user_data(btn);
    if (idx < 0 || idx >= APP_SMART_HOME_COUNT) return;

    int lv = app_smart_home_toggle(idx);
    lv_obj_set_style_bg_color(btn, lv ? SH_COLOR_HIGH : SH_COLOR_LOW, 0);
}

// 长按弹编辑名字面板（手动注册 LV_EVENT_LONG_PRESSED）
static void _btn_long_press_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_current_target(e);
    int idx = (int)(uintptr_t)lv_obj_get_user_data(btn);
    if (idx < 0 || idx >= APP_SMART_HOME_COUNT) return;

    // 已有面板则先关闭
    if (s_edit_cont) _edit_cleanup();

    s_edit_idx = idx;

    // 主容器
    s_edit_cont = lv_obj_create(lv_screen_active());
    lv_obj_set_size(s_edit_cont, 500, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(s_edit_cont, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(s_edit_cont, LV_ALIGN_CENTER, 0, -80);

    // 标题
    lv_obj_t *title = lv_label_create(s_edit_cont);
    lv_label_set_text(title, "编辑名字");
    lv_obj_set_style_text_font(title, font_chinese_6500_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    // 输入框：预填当前名字
    s_edit_ta = lv_textarea_create(s_edit_cont);
    lv_textarea_set_one_line(s_edit_ta, true);
    lv_textarea_set_text(s_edit_ta, app_smart_home_get_name(idx));
    lv_obj_set_style_text_font(s_edit_ta, font_chinese_6500_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_width(s_edit_ta, 460);
    lv_obj_align(s_edit_ta, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_add_event_cb(s_edit_ta, _edit_ready_cb, LV_EVENT_READY, NULL);
    lv_obj_add_event_cb(s_edit_ta, _edit_cancel_cb, LV_EVENT_CANCEL, NULL);

    // 预设中文标签行（flex row）
    lv_obj_t *preset_row = lv_obj_create(s_edit_cont);
    lv_obj_set_size(preset_row, 460, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(preset_row, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(preset_row, 0, 0);
    lv_obj_set_style_pad_all(preset_row, 0, 0);
    lv_obj_set_style_pad_column(preset_row, 6, 0);
    lv_obj_set_style_layout(preset_row, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(preset_row, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(preset_row, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(preset_row, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_align(preset_row, LV_ALIGN_TOP_MID, 0, 70);

    for (int i = 0; i < (int)SH_PRESET_COUNT; i++)
    {
        lv_obj_t *pbtn = lv_button_create(preset_row);
        lv_obj_set_size(pbtn, 60, 32);
        lv_obj_set_user_data(pbtn, (void *)(uintptr_t)i);
        lv_obj_add_event_cb(pbtn, _preset_name_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t *plbl = lv_label_create(pbtn);
        lv_label_set_text(plbl, s_preset_names[i]);
        lv_obj_set_style_text_font(plbl, font_chinese_6500_14, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_center(plbl);
    }

    // 键盘（textarea + keyboard 双重注册取消回调，防止键盘事件未传播）
    s_edit_kb = lv_keyboard_create(lv_screen_active());
    lv_keyboard_set_textarea(s_edit_kb, s_edit_ta);
    lv_obj_add_event_cb(s_edit_kb, _edit_cancel_cb, LV_EVENT_CANCEL, NULL);
}

// 预设标签点击 → 填入 textarea（不立即提交，等 ENTER 确认）
static void _preset_name_cb(lv_event_t *e)
{
    lv_obj_t *pbtn = lv_event_get_current_target(e);
    int i = (int)(uintptr_t)lv_obj_get_user_data(pbtn);
    if (i < 0 || i >= (int)SH_PRESET_COUNT) return;
    if (s_edit_ta) lv_textarea_set_text(s_edit_ta, s_preset_names[i]);
}

// 确认改名（textarea READY / 键盘 ENTER）
static void _edit_ready_cb(lv_event_t *e)
{
    (void)e;
    // 必须在 cleanup 前拷贝文本，textarea 删除后指针失效
    char name[APP_SMART_HOME_NAME_MAX];
    strlcpy(name, lv_textarea_get_text(s_edit_ta), sizeof(name));

    app_smart_home_set_name(s_edit_idx, name);
    int idx = s_edit_idx;
    _edit_cleanup();
    _refresh_btn(idx);
}

// 取消
static void _edit_cancel_cb(lv_event_t *e)
{
    (void)e;
    _edit_cleanup();
}

// 清理弹窗（删 keyboard + 主容器，置 NULL）
static void _edit_cleanup(void)
{
    if (s_edit_kb)
    {
        lv_obj_delete(s_edit_kb);
        s_edit_kb = NULL;
    }
    if (s_edit_cont)
    {
        lv_obj_delete(s_edit_cont);
        s_edit_cont = NULL;
        s_edit_ta = NULL;
    }
}

// 恢复默认（XML event_cb 绑定 action_sh_reset）
void action_sh_reset(lv_event_t *e)
{
    (void)e;
    app_smart_home_reset_default();
    _refresh_all();
}

// 刷新单个按钮：背景色按电平，文字按名字
static void _refresh_btn(int idx)
{
    char namebuf[16];
    snprintf(namebuf, sizeof(namebuf), "sh_btn_%d", idx);
    lv_obj_t *btn = lv_obj_find_by_name(smart_home, namebuf);
    if (btn)
    {
        int lv = app_smart_home_get_level(idx);
        lv_obj_set_style_bg_color(btn, lv ? SH_COLOR_HIGH : SH_COLOR_LOW, 0);
    }

    snprintf(namebuf, sizeof(namebuf), "sh_lbl_%d", idx);
    lv_obj_t *lbl = lv_obj_find_by_name(smart_home, namebuf);
    if (lbl)
    {
        lv_label_set_text(lbl, app_smart_home_get_name(idx));
    }
}

// 刷新全部按钮
static void _refresh_all(void)
{
    for (int i = 0; i < APP_SMART_HOME_COUNT; i++)
    {
        _refresh_btn(i);
    }
}
