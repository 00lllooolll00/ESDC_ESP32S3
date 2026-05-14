#include "styles.h"
#include "common_header.h"
#include "plat_wifi.h"
#include "app_wifi.h"
#include "screens.h"

FILE_TAG("actions");

// 缓存扫描结果，用于点击按钮时查找 SSID
static plat_wifi_ap_info_t s_scan_results[WIFI_SCAN_MAX_AP];
static int s_scan_count;

// 密码输入 UI
static lv_obj_t *s_password_cont;
static lv_obj_t *s_password_ta;
static lv_obj_t *s_password_kb;

// 连接状态
static char s_connect_ssid[33];
static bool s_connecting;
static lv_obj_t *s_connect_status_item;
static lv_timer_t *s_status_timer;

static void _wifi_evt_cb(app_wifi_evt_t evt, void *data, void *arg);
static void _wifi_btn_click_cb(lv_event_t *e);
static void _password_ready_cb(lv_event_t *e);
static void _password_cancel_cb(lv_event_t *e);
static void _cleanup_password_ui(void);
static void _status_timer_cb(lv_timer_t *timer);
static void _clear_connect_status(void);
static void _show_connect_status(const char *text);
static void _finish_connect_attempt(const char *text, lv_color_t color);

// ============================================================================
// Public
// ============================================================================

void action_wifi_start_scan(lv_event_t *e)
{
    lv_obj_t *list = objects.obj0__wifi_list;

    s_scan_count = 0;
    s_connecting = false;
    s_connect_ssid[0] = '\0';
    s_connect_status_item = NULL;

    // 清除上一次扫描结果（保留 spinner）
    uint32_t child_cnt = lv_obj_get_child_cnt(list);
    for (int32_t i = (int32_t)child_cnt - 1; i >= 0; i--)
    {
        lv_obj_t *child = lv_obj_get_child(list, i);
        if (child != objects.obj0__wifi_loader) lv_obj_delete(child);
    }

    // 显示 spinner
    lv_obj_clear_flag(objects.obj0__wifi_loader, LV_OBJ_FLAG_HIDDEN);

    // 注册事件回调
    app_wifi_register_evt_cb(_wifi_evt_cb, NULL);

    // 发送扫描命令
    app_wifi_cmd_msg_t msg = { .cmd = APP_WIFI_CMD_SCAN };
    if (app_wifi_send_cmd(&msg, pdMS_TO_TICKS(1000)) != 0)
    {
        lv_obj_add_flag(objects.obj0__wifi_loader, LV_OBJ_FLAG_HIDDEN);
        lv_list_add_btn(list, NULL, "Send cmd failed");
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
    if (idx < 0 || idx >= s_scan_count) return;

    strlcpy(s_connect_ssid, s_scan_results[idx].ssid, sizeof(s_connect_ssid));

    // 容器
    s_password_cont = lv_obj_create(lv_screen_active());
    lv_obj_set_size(s_password_cont, 500, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(s_password_cont, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(s_password_cont, LV_ALIGN_CENTER, 0, -80);

    // 标题
    lv_obj_t *label = lv_label_create(s_password_cont);
    lv_label_set_text_fmt(label, "Password for %s", s_connect_ssid);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    // 密码输入框
    s_password_ta = lv_textarea_create(s_password_cont);
    lv_textarea_set_one_line(s_password_ta, true);
    lv_textarea_set_password_mode(s_password_ta, true);
    lv_textarea_set_placeholder_text(s_password_ta, "Enter password");
    lv_obj_set_width(s_password_ta, 460);
    lv_obj_align(s_password_ta, LV_ALIGN_TOP_MID, 0, 30);

    lv_obj_add_event_cb(s_password_ta, _password_ready_cb, LV_EVENT_READY, NULL);
    lv_obj_add_event_cb(s_password_ta, _password_cancel_cb, LV_EVENT_CANCEL, NULL);

    // 键盘（textarea + keyboard 双重注册取消回调，防止键盘事件未传播）
    s_password_kb = lv_keyboard_create(lv_screen_active());
    lv_keyboard_set_textarea(s_password_kb, s_password_ta);
    lv_obj_add_event_cb(s_password_kb, _password_cancel_cb, LV_EVENT_CANCEL, NULL);
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

    _cleanup_password_ui();
    s_connecting = true;
    _show_connect_status("Connecting...");

    app_wifi_cmd_msg_t msg = { .cmd = APP_WIFI_CMD_CONNECT };
    strlcpy(msg.data.connect.ssid, s_connect_ssid, sizeof(msg.data.connect.ssid));
    strlcpy(msg.data.connect.password, password, sizeof(msg.data.connect.password));
    if (app_wifi_send_cmd(&msg, pdMS_TO_TICKS(1000)) != 0)
    {
        _finish_connect_attempt("Send cmd failed", lv_color_hex(0xFF1744));
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

static void _status_timer_cb(lv_timer_t *timer)
{
    _clear_connect_status();
}

static void _clear_connect_status(void)
{
    if (s_status_timer)
    {
        lv_timer_delete(s_status_timer);
        s_status_timer = NULL;
    }
    if (s_connect_status_item)
    {
        lv_obj_delete(s_connect_status_item);
        s_connect_status_item = NULL;
    }
}

static void _show_connect_status(const char *text)
{
    _clear_connect_status();
    s_connect_status_item = lv_list_add_btn(objects.obj0__wifi_list, NULL, text);
    s_status_timer = lv_timer_create(_status_timer_cb, 3000, NULL);
    lv_timer_set_repeat_count(s_status_timer, 1);
}

static void _finish_connect_attempt(const char *text, lv_color_t color)
{
    _show_connect_status(text);
    lv_obj_set_style_text_color(s_connect_status_item, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    s_connecting = false;
    s_connect_ssid[0] = '\0';
}

// ============================================================================
// WiFi 事件回调（扫描结果 + 连接状态）
// ============================================================================

static void _wifi_evt_cb(app_wifi_evt_t evt, void *data, void *arg)
{
    lv_obj_t *list = objects.obj0__wifi_list;

    switch (evt)
    {
        case APP_WIFI_EVT_SCAN_DONE:
            {
                app_wifi_scan_result_t *result = (app_wifi_scan_result_t *)data;

                lv_obj_add_flag(objects.obj0__wifi_loader, LV_OBJ_FLAG_HIDDEN);

                s_scan_count = result->count;
                for (int i = 0; i < result->count; i++)
                {
                    s_scan_results[i] = result->aps[i];

                    char buf[48];
                    snprintf(buf, sizeof(buf), "%s", result->aps[i].ssid);
                    lv_obj_t *btn = lv_list_add_btn(list, LV_SYMBOL_WIFI, buf);
                    add_style_wifi_scan_result(btn);
                    lv_obj_set_user_data(btn, (void *)(uintptr_t)i);
                    lv_obj_add_event_cb(btn, _wifi_btn_click_cb, LV_EVENT_CLICKED, NULL);
                }

                free(result);
                break;
            }

        case APP_WIFI_EVT_SCAN_FAIL:
            lv_obj_add_flag(objects.obj0__wifi_loader, LV_OBJ_FLAG_HIDDEN);
            lv_list_add_btn(list, NULL, "Scan failed");
            break;

        case APP_WIFI_EVT_CONNECTING:
            if (s_connecting)
            {
                _show_connect_status("Connecting...");
            }
            break;

        case APP_WIFI_EVT_CONNECTED:
            if (s_connecting)
            {
                _finish_connect_attempt("Connected", lv_color_hex(0x00C853));
            }
            break;

        case APP_WIFI_EVT_DISCONNECTED:
            if (s_connecting)
            {
                _finish_connect_attempt("Password error", lv_color_hex(0xFF1744));
            }
            break;

        default:
            break;
    }
}
