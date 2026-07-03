#include "common_header.h"
#include "ui.h"
#include "plat_wifi.h"
#include "app_wifi.h"
#include "app_ip_location.h"
#include "app_wifi_store.h"

EK_LOG_FILE_TAG("wifi_actions");

// 缓存扫描结果，用于点击按钮时查找 SSID
static plat_wifi_ap_info_t s_scan_results[WIFI_SCAN_MAX_AP];
static int s_scan_count;

// 密码输入 UI
static lv_obj_t *s_password_cont;
static lv_obj_t *s_password_ta;
static lv_obj_t *s_password_kb;

// 连接状态
static char s_connect_ssid[33];
static char s_connect_password[64];
static bool s_connecting;
static lv_obj_t *s_connect_status_item;

// 缓存的 widget 指针（每次屏幕加载时更新）
static lv_obj_t *s_wifi_list;
static lv_obj_t *s_wifi_loader;
static lv_obj_t *s_wifi_list_panel;

static void _wifi_evt_cb(app_wifi_evt_t evt, void *data, void *arg);
static void _wifi_btn_click_cb(lv_event_t *e);
static void _wifi_forget_cb(lv_event_t *e);
static void _password_ready_cb(lv_event_t *e);
static void _password_cancel_cb(lv_event_t *e);
static void _cleanup_password_ui(void);
static void _clear_connect_status(void);
static void _show_connect_status(const char *text);
static void _finish_connect_attempt(const char *text, lv_color_t color);

// 面板显示/隐藏回调（show_wifi_panel / hide_wifi_panel）已在 ui.c 中实现

// ============================================================================
// Public
// ============================================================================

void action_wifi_start_scan(lv_event_t *e)
{
    // 查找当前屏幕的 wifi_list 和 wifi_loader
    s_wifi_list = lv_obj_find_by_name(lv_screen_active(), "wifi_list");
    s_wifi_loader = lv_obj_find_by_name(lv_screen_active(), "wifi_loader");
    s_wifi_list_panel = lv_obj_find_by_name(lv_screen_active(), "wifi_list_panel");

    lv_obj_t *list = s_wifi_list;

    s_scan_count = 0;
    s_connecting = false;
    s_connect_ssid[0] = '\0';
    _clear_connect_status();

    // 清除上一次扫描结果（保留 spinner）
    uint32_t child_cnt = lv_obj_get_child_cnt(list);
    for (int32_t i = (int32_t)child_cnt - 1; i >= 0; i--)
    {
        lv_obj_t *child = lv_obj_get_child(list, i);
        if (child != s_wifi_loader)
        {
            lv_obj_delete(child);
        }
    }

    // 显示 spinner
    if (s_wifi_loader)
    {
        lv_obj_clear_flag(s_wifi_loader, LV_OBJ_FLAG_HIDDEN);
    }

    // 注册事件回调
    app_wifi_register_evt_cb(_wifi_evt_cb, NULL);

    // 发送扫描命令
    app_wifi_cmd_msg_t msg = {.cmd = APP_WIFI_CMD_SCAN};
    if (app_wifi_send_cmd(&msg, pdMS_TO_TICKS(1000)) != 0)
    {
        if (s_wifi_loader)
        {
            lv_obj_add_flag(s_wifi_loader, LV_OBJ_FLAG_HIDDEN);
        }
        lv_obj_t *btn = lv_list_add_btn(list, NULL, "发送失败");
        (void)btn;
        app_wifi_unregister_evt_cb();
    }
}

// ============================================================================
// WiFi 列表项点击 → 弹出密码键盘
// ============================================================================

static void _wifi_btn_click_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_current_target(e);
    int idx = (int)(uintptr_t)lv_obj_get_user_data(btn);
    if (idx < 0 || idx >= s_scan_count)
    {
        return;
    }

    strlcpy(s_connect_ssid, s_scan_results[idx].ssid, sizeof(s_connect_ssid));

    // 已记住密码 → 直接连接，免输入
    char saved_pwd[64];
    if (app_wifi_store_find(s_connect_ssid, saved_pwd, sizeof(saved_pwd)))
    {
        strlcpy(s_connect_password, saved_pwd, sizeof(s_connect_password));
        s_connecting = true;
        _show_connect_status("连接中...");

        app_wifi_cmd_msg_t msg = {.cmd = APP_WIFI_CMD_CONNECT};
        strlcpy(msg.data.connect.ssid, s_connect_ssid, sizeof(msg.data.connect.ssid));
        strlcpy(msg.data.connect.password, saved_pwd, sizeof(msg.data.connect.password));
        if (app_wifi_send_cmd(&msg, pdMS_TO_TICKS(1000)) != 0)
        {
            _finish_connect_attempt("发送失败", lv_color_hex(0xFF1744));
        }
        return;
    }

    // 未记住 → 弹密码键盘
    // 容器
    s_password_cont = lv_obj_create(lv_screen_active());
    lv_obj_set_size(s_password_cont, 500, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(s_password_cont, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(s_password_cont, LV_ALIGN_CENTER, 0, -80);

    // 标题
    lv_obj_t *label = lv_label_create(s_password_cont);
    lv_label_set_text_fmt(label, "输入密码: %s", s_connect_ssid);
    lv_obj_set_style_text_font(label, chinese_3500_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    // 密码输入框
    s_password_ta = lv_textarea_create(s_password_cont);
    lv_textarea_set_one_line(s_password_ta, true);
    lv_textarea_set_password_mode(s_password_ta, true);
    lv_textarea_set_placeholder_text(s_password_ta, "请输入密码");
    lv_obj_set_style_text_font(s_password_ta, chinese_3500_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_width(s_password_ta, 460);
    lv_obj_align(s_password_ta, LV_ALIGN_TOP_MID, 0, 30);

    lv_obj_add_event_cb(s_password_ta, _password_ready_cb, LV_EVENT_READY, NULL);
    lv_obj_add_event_cb(s_password_ta, _password_cancel_cb, LV_EVENT_CANCEL, NULL);

    // 键盘（textarea + keyboard 双重注册取消回调，防止键盘事件未传播）
    s_password_kb = lv_keyboard_create(lv_screen_active());
    lv_keyboard_set_textarea(s_password_kb, s_password_ta);
    lv_obj_add_event_cb(s_password_kb, _password_cancel_cb, LV_EVENT_CANCEL, NULL);
}

// 忘记密码：删除凭据并重新扫描刷新列表
static void _wifi_forget_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_current_target(e);
    int idx = (int)(uintptr_t)lv_obj_get_user_data(btn);
    if (idx < 0 || idx >= s_scan_count)
    {
        return;
    }
    app_wifi_store_remove(s_scan_results[idx].ssid);
    // 重新扫描刷新列表（该 WiFi 仍在扫描结果，但变为未记住状态）
    action_wifi_start_scan(NULL);
}

// ============================================================================
// 密码确认 → 发起连接
// ============================================================================

static void _password_ready_cb(lv_event_t *e)
{
    // 必须在 _cleanup_password_ui 之前复制密码，
    // 否则 textarea 被删除后指针失效
    char password[64];
    strlcpy(password, lv_textarea_get_text(s_password_ta), sizeof(password));
    strlcpy(s_connect_password, password, sizeof(s_connect_password));

    _cleanup_password_ui();
    s_connecting = true;
    _show_connect_status("连接中...");

    app_wifi_cmd_msg_t msg = {.cmd = APP_WIFI_CMD_CONNECT};
    strlcpy(msg.data.connect.ssid, s_connect_ssid, sizeof(msg.data.connect.ssid));
    strlcpy(msg.data.connect.password, password, sizeof(msg.data.connect.password));
    if (app_wifi_send_cmd(&msg, pdMS_TO_TICKS(1000)) != 0)
    {
        _finish_connect_attempt("发送失败", lv_color_hex(0xFF1744));
    }
}

// ============================================================================
// 取消 → 关闭键盘
// ============================================================================

static void _password_cancel_cb(lv_event_t *e)
{
    _cleanup_password_ui();
}

static void _cleanup_password_ui(void)
{
    if (s_password_kb)
    {
        lv_obj_delete(s_password_kb);
        s_password_kb = NULL;
    }
    if (s_password_cont)
    {
        lv_obj_delete(s_password_cont);
        s_password_cont = NULL;
        s_password_ta = NULL;
    }
}

static void _clear_connect_status(void)
{
    if (s_connect_status_item)
    {
        lv_obj_delete(s_connect_status_item);
        s_connect_status_item = NULL;
    }
}

static void _show_connect_status(const char *text)
{
    _clear_connect_status();

    s_connect_status_item = lv_label_create(s_wifi_list_panel);
    lv_label_set_text(s_connect_status_item, text);
    lv_obj_set_style_text_font(s_connect_status_item, chinese_3500_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(s_connect_status_item, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(s_connect_status_item, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(s_connect_status_item, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(s_connect_status_item, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(s_connect_status_item, LV_ALIGN_CENTER, 0, 0);
}

static void _finish_connect_attempt(const char *text, lv_color_t color)
{
    _show_connect_status(text);
    lv_obj_set_style_text_color(s_connect_status_item, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(s_connect_status_item, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(s_connect_status_item, LV_OPA_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    s_connecting = false;
    s_connect_ssid[0] = '\0';
}

// ============================================================================
// WiFi 事件回调（扫描结果 + 连接状态）
// ============================================================================

static void _wifi_evt_cb(app_wifi_evt_t evt, void *data, void *arg)
{
    lv_obj_t *list = s_wifi_list;

    switch (evt)
    {
        case APP_WIFI_EVT_SCAN_DONE:
        {
            app_wifi_scan_result_t *result = (app_wifi_scan_result_t *)data;

            if (s_wifi_loader)
            {
                lv_obj_add_flag(s_wifi_loader, LV_OBJ_FLAG_HIDDEN);
            }

            s_scan_count = result->count;
            for (int i = 0; i < result->count; i++)
            {
                s_scan_results[i] = result->aps[i];

                char buf[48];
                snprintf(buf, sizeof(buf), "%s", result->aps[i].ssid);
                lv_obj_t *btn = lv_list_add_btn(list, LV_SYMBOL_WIFI, buf);
                lv_obj_t *label = lv_obj_get_child(btn, 1);

                if (label)
                {
                    lv_obj_set_style_text_font(label, chinese_3500_14, 0);
                }

                lv_obj_set_user_data(btn, (void *)(uintptr_t)i);
                lv_obj_add_event_cb(btn, _wifi_btn_click_cb, LV_EVENT_CLICKED, NULL);

                // 已记住的项：加“忘记”按钮
                char _tmp_pwd[64];
                if (app_wifi_store_find(result->aps[i].ssid, _tmp_pwd, sizeof(_tmp_pwd)))
                {
                    lv_obj_t *forget_btn = lv_button_create(btn);
                    lv_obj_set_size(forget_btn, 50, 30);
                    lv_obj_set_style_bg_color(forget_btn, lv_color_hex(0xFF1744), 0);
                    lv_obj_t *flbl = lv_label_create(forget_btn);
                    lv_label_set_text(flbl, "忘记");
                    lv_obj_set_style_text_font(flbl, chinese_3500_14, 0);
                    lv_obj_center(flbl);
                    lv_obj_set_user_data(forget_btn, (void *)(uintptr_t)i);
                    lv_obj_add_event_cb(forget_btn, _wifi_forget_cb, LV_EVENT_CLICKED, NULL);
                }
            }

            free(result);
            break;
        }

        case APP_WIFI_EVT_SCAN_FAIL:
            if (s_wifi_loader)
            {
                lv_obj_add_flag(s_wifi_loader, LV_OBJ_FLAG_HIDDEN);
            }
            lv_list_add_btn(list, NULL, "扫描失败");
            break;

        case APP_WIFI_EVT_CONNECTING:
            if (s_connecting)
            {
                _show_connect_status("连接中...");
            }
            break;

        case APP_WIFI_EVT_CONNECTED:
        {
            lv_subject_set_int(&wifi_is_connected, 1);
            app_ip_location_request();
            app_wifi_store_add(s_connect_ssid, s_connect_password);
            // 更新 wifi_state 图标为已连接（勾）
            lv_obj_t *state = lv_obj_find_by_name(lv_screen_active(), "wifi_state");
            if (state) lv_label_set_text(state, LV_SYMBOL_OK);
            if (s_connecting)
            {
                _finish_connect_attempt("连接成功", lv_color_hex(0x00C853));
            }
            break;
        }

        case APP_WIFI_EVT_DISCONNECTED:
        {
            lv_subject_set_int(&wifi_is_connected, 0);
            // 更新 wifi_state 图标为未连接（叉）
            lv_obj_t *state = lv_obj_find_by_name(lv_screen_active(), "wifi_state");
            if (state) lv_label_set_text(state, LV_SYMBOL_CLOSE);
            if (s_connecting)
            {
                _finish_connect_attempt("密码错误", lv_color_hex(0xFF1744));
            }
            break;
        }

        default:
            break;
    }
}
