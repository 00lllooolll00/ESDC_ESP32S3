#include "ui_scrs.h"
#include "app_wifi_msg.h"

FILE_TAG("ui_scr_wifi.c");

static void _scan_btn_event_cb(lv_event_t *e);
static void _list_btn_event_cb(lv_event_t *e);
static void _kb_event_cb(lv_event_t *e);
static void _ui_show_password(const char *ssid);
static void _wifi_evt_cb(app_wifi_evt_t evt, void *data, void *arg);

static lv_obj_t *s_wifi_list;
static lv_obj_t *s_pw_container;
static lv_obj_t *s_ta_passwd;
static lv_obj_t *s_kb;
static char s_cur_ssid[33];

void ui_wifi_scr_create(void)
{
    lv_obj_t *scr = lv_screen_active();
    lv_obj_t *btn_scan = lv_button_create(scr);
    lv_obj_align(btn_scan, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_event_cb(btn_scan, _scan_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label_scan = lv_label_create(btn_scan);
    lv_label_set_text(label_scan, "Scan Wi-Fi");
    lv_obj_align(label_scan, LV_ALIGN_CENTER, 0, 0);

    s_wifi_list = lv_list_create(scr);
    lv_obj_align(s_wifi_list, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_size(s_wifi_list, 280, 200);

    lv_list_add_text(s_wifi_list, "Press Scan");

    app_wifi_register_evt_cb(_wifi_evt_cb, NULL);
}

static void _wifi_evt_cb(app_wifi_evt_t evt, void *data, void *arg)
{
    switch (evt)
    {
        case APP_WIFI_EVT_SCAN_DONE:
            {
                app_wifi_scan_result_t *result = (app_wifi_scan_result_t *)data;
                lv_obj_clean(s_wifi_list);
                for (int i = 0; i < result->count; i++)
                {
                    lv_obj_t *btn = lv_list_add_button(s_wifi_list, LV_SYMBOL_WIFI, result->aps[i].ssid);
                    lv_obj_add_event_cb(btn, _list_btn_event_cb, LV_EVENT_CLICKED, NULL);
                }
                free(result);
                break;
            }
        case APP_WIFI_EVT_SCAN_FAIL:
            lv_obj_clean(s_wifi_list);
            lv_list_add_text(s_wifi_list, "Scan failed");
            break;
        case APP_WIFI_EVT_CONNECTED:
            LOG_INFO("wifi connected - UI notified");
            break;
        case APP_WIFI_EVT_DISCONNECTED:
            LOG_INFO("wifi disconnected - UI notified");
            break;
        case APP_WIFI_EVT_CONNECTING:
            LOG_INFO("wifi connecting - UI notified");
            break;
    }
}

static void _scan_btn_event_cb(lv_event_t *e)
{
    lv_obj_clean(s_wifi_list);
    lv_list_add_text(s_wifi_list, "Scanning...");

    app_wifi_cmd_msg_t msg = {
        .cmd = APP_WIFI_CMD_SCAN,
    };
    if (app_wifi_send_cmd(&msg, pdMS_TO_TICKS(100)) != 0)
    {
        lv_obj_clean(s_wifi_list);
        lv_list_add_text(s_wifi_list, "Scan busy");
    }
}

static void _ui_show_password(const char *ssid)
{
    lv_obj_t *scr = lv_screen_active();
    s_pw_container = lv_obj_create(scr);
    lv_obj_set_size(s_pw_container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(s_pw_container, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(s_pw_container, LV_OPA_50, 0);

    lv_obj_t *title = lv_label_create(s_pw_container);
    lv_label_set_text_fmt(title, "Connect to: %s", ssid);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    s_ta_passwd = lv_textarea_create(s_pw_container);
    lv_obj_set_width(s_ta_passwd, 240);
    lv_obj_align(s_ta_passwd, LV_ALIGN_TOP_MID, 0, 50);
    lv_textarea_set_password_mode(s_ta_passwd, true);
    lv_textarea_set_one_line(s_ta_passwd, true);
    lv_textarea_set_placeholder_text(s_ta_passwd, "Enter Password");

    s_kb = lv_keyboard_create(s_pw_container);
    lv_obj_align(s_kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_keyboard_set_textarea(s_kb, s_ta_passwd);

    lv_obj_add_event_cb(s_kb, _kb_event_cb, LV_EVENT_READY, NULL);
    lv_obj_add_event_cb(s_kb, _kb_event_cb, LV_EVENT_CANCEL, NULL);
}

static void _list_btn_event_cb(lv_event_t *e)
{
    lv_obj_t *clicked_btn = lv_event_get_target(e);
    const char *ssid = lv_list_get_button_text(s_wifi_list, clicked_btn);

    if (ssid != NULL)
    {
        strncpy(s_cur_ssid, ssid, sizeof(s_cur_ssid) - 1);
        s_cur_ssid[sizeof(s_cur_ssid) - 1] = '\0';
        _ui_show_password(s_cur_ssid);
    }
}

static void _kb_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_READY)
    {
        const char *password = lv_textarea_get_text(s_ta_passwd);

        app_wifi_cmd_msg_t msg = {
            .cmd = APP_WIFI_CMD_CONNECT,
        };
        strncpy(msg.data.connect.ssid, s_cur_ssid, sizeof(msg.data.connect.ssid) - 1);
        msg.data.connect.ssid[sizeof(msg.data.connect.ssid) - 1] = '\0';
        if (password)
        {
            strncpy(msg.data.connect.password, password, sizeof(msg.data.connect.password) - 1);
            msg.data.connect.password[sizeof(msg.data.connect.password) - 1] = '\0';
        }

        app_wifi_send_cmd(&msg, pdMS_TO_TICKS(100));

        lv_obj_delete(s_pw_container);
        s_pw_container = NULL;
    }
    else if (code == LV_EVENT_CANCEL)
    {
        lv_obj_delete(s_pw_container);
        s_pw_container = NULL;
    }
}
