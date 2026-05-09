#include "ui_scrs.h"
#include "plat_wifi.h"

FILE_TAG("ui_scr_wifi.c");

// 扫描按钮的回调函数
static void _scan_btn_event_cb(lv_event_t *e);
// 列表项点击的回调函数
static void _list_btn_event_cb(lv_event_t *e);
// 键盘“确认”或“取消”按钮的回调
static void _kb_event_cb(lv_event_t *e);
// 显示wifi输入密码的函数
static void _ui_show_password(const char *ssid);

static lv_obj_t *s_wifi_list; // wifi 列表
static lv_obj_t *s_pw_container; // 输入密码的容器
static lv_obj_t *s_ta_passwd; // 密码输入框
static lv_obj_t *s_kb; // 键盘
static char s_cur_ssid[64]; // 当前存储的 ssid

extern plat_wifi_dev_t g_wifi_dev;

void ui_wifi_scr_create(void)
{
    lv_obj_t *scr = lv_screen_active();
    lv_obj_t *btn_scan = lv_button_create(scr);
    lv_obj_align(btn_scan, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_event_cb(btn_scan, _scan_btn_event_cb, LV_EVENT_TRIPLE_CLICKED, NULL);

    lv_obj_t *label_scan = lv_label_create(btn_scan);
    lv_label_set_text(label_scan, "Scan Wi-Fi");
    lv_obj_align(label_scan, LV_ALIGN_CENTER, 0, 0);

    s_wifi_list = lv_list_create(scr);
    lv_obj_align(s_wifi_list, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_size(s_wifi_list, 280, 200);
}

static void _ui_show_password(const char *ssid)
{
    lv_obj_t *scr = lv_screen_active();
    s_pw_container = lv_obj_create(scr);
    lv_obj_set_size(s_pw_container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(s_pw_container, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(s_pw_container, LV_OPA_50, 0); // 半透明背景
    // 提示文本 (显示正在连接的 SSID)
    lv_obj_t *title = lv_label_create(s_pw_container);
    lv_label_set_text_fmt(title, "Connect to: %s", ssid);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // 密码输入框
    s_ta_passwd = lv_textarea_create(s_pw_container);
    lv_obj_set_width(s_ta_passwd, 240);
    lv_obj_align(s_ta_passwd, LV_ALIGN_TOP_MID, 0, 50);
    lv_textarea_set_password_mode(s_ta_passwd, true); // 设置为密码模式（显示*号）
    lv_textarea_set_one_line(s_ta_passwd, true);
    lv_textarea_set_placeholder_text(s_ta_passwd, "Enter Password");

    // 虚拟键盘
    s_kb = lv_keyboard_create(s_pw_container);
    lv_obj_align(s_kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_keyboard_set_textarea(s_kb, s_ta_passwd); // 将键盘与文本框绑定

    // 监听键盘的 Ready(确认) 和 Cancel(取消) 事件
    lv_obj_add_event_cb(s_kb, _kb_event_cb, LV_EVENT_READY, NULL);
    lv_obj_add_event_cb(s_kb, _kb_event_cb, LV_EVENT_CANCEL, NULL);
}

static void _scan_btn_event_cb(lv_event_t *e)
{
    // 1. 清空当前列表
    lv_obj_clean(s_wifi_list);

    // 提示正在扫描
    lv_obj_t *loading_label = lv_list_add_text(s_wifi_list, "Scanning...");

    // TODO: 触发底层 Wi-Fi 扫描
    // NOTE：如果是阻塞式扫描，不要直接在这里写死，否则会导致 UI 卡顿。
    // SUGGEST：这里发送一个 FreeRTOS 信号量给 Wi-Fi 任务，扫描完成后再通过回调刷新 UI。
    plat_wifi_ap_info_t info;
    plat_wifi_scan(&g_wifi_dev, &info, 20);

    // 【模拟】扫描完成后刷新列表 (实际应用中应在底层扫描完成后的回调中执行)
    lv_obj_clean(s_wifi_list);

    // 假设这是你从 ESP32 API 获取到的 SSID 列表
    const char *scanned_ssids[] = { "MyHome_g_WiFi", "Office_5G", "Guest_Network" };

    for (int i = 0; i < 3; i++)
    {
        // 添加带有 Wi-Fi 图标的按钮
        lv_obj_t *btn = lv_list_add_button(s_wifi_list, LV_SYMBOL_WIFI, scanned_ssids[i]);
        // 为每个项绑定点击事件
        lv_obj_add_event_cb(btn, _list_btn_event_cb, LV_EVENT_CLICKED, NULL);
    }
}

static void _list_btn_event_cb(lv_event_t *e)
{
    lv_obj_t *clicked_btn = lv_event_get_target(e);

    // 获取按钮上的文本 (即 Wi-Fi 名称)
    const char *ssid = lv_list_get_button_text(s_wifi_list, clicked_btn);

    if (ssid != NULL)
    {
        strncpy(s_cur_ssid, ssid, sizeof(s_cur_ssid) - 1);
        s_cur_ssid[sizeof(s_cur_ssid) - 1] = '\0';

        // 弹出密码输入界面
        _ui_show_password(s_cur_ssid);
    }
}

static void _kb_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_READY)
    {
        // 用户点击了键盘上的 "Enter/确认"
        const char *password = lv_textarea_get_text(s_ta_passwd);

        // TODO:在这里调用 Wi-Fi 连接 API
        TODO("api:wifi connect");
        LOG_INFO("connecting to %s with password: %s", s_cur_ssid, password);

        // 销毁密码界面
        lv_obj_delete(s_pw_container);
        s_pw_container = NULL;
    }
    else if (code == LV_EVENT_CANCEL)
    {
        // 用户点击了取消
        lv_obj_delete(s_pw_container);
        s_pw_container = NULL;
    }
}
