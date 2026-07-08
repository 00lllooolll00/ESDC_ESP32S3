#include "common_header.h"
#include "ui.h"
#include "app_ai_chat.h"

EK_LOG_FILE_TAG("ai_chat_actions");

#define VOICE_CANCEL_THRESHOLD 30 // 上滑超过 30px 触发取消
#define MAX_BUBBLES            20 // 消息列表最多保留 20 条

// 长按手势状态
typedef enum {
    VOICE_STATE_IDLE,
    VOICE_STATE_RECORDING,
    VOICE_STATE_CANCEL,
} voice_state_t;

// 缓存的控件指针
static lv_obj_t *s_msg_list;
static lv_obj_t *s_voice_btn;
static lv_obj_t *s_voice_label;
static lv_obj_t *s_loading_label;


// 手势状态
static voice_state_t s_voice_state = VOICE_STATE_IDLE;
static lv_coord_t s_press_start_y;
static bool s_long_pressed;

// 前向声明
static void _voice_pressed(lv_event_t *e);
static void _voice_long_press(lv_event_t *e);
static void _voice_pressing(lv_event_t *e);
static void _voice_released(lv_event_t *e);
static void _set_voice_state(voice_state_t state);
static void _ai_reply_cb(const char *reply, void *arg);
static void _add_message(const char *text, bool is_user);
static void _scroll_to_bottom(void);

// ============================================================================
// Public
// ============================================================================

void ai_chat_actions_init(void)
{
    // 获取控件指针
    s_msg_list = lv_obj_find_by_name(ai_chat, "ai_chat_msg_list");
    s_voice_btn = lv_obj_find_by_name(ai_chat, "ai_chat_voice_btn");
    s_voice_label = lv_obj_find_by_name(ai_chat, "ai_chat_voice_label");
    s_loading_label = lv_obj_find_by_name(ai_chat, "ai_chat_loading");

    // 消息列表底部对齐（新消息在底部）
    if (s_msg_list)
    {
        lv_obj_set_style_flex_main_place(s_msg_list, LV_FLEX_ALIGN_END, 0);
    }

    // 隐藏 loading 提示（XML 中 hidden 可能未生效）
    if (s_loading_label)
    {
        lv_obj_add_flag(s_loading_label, LV_OBJ_FLAG_HIDDEN);
    }

    // 注册长按手势事件
    if (s_voice_btn)
    {
        lv_obj_add_event_cb(s_voice_btn, _voice_pressed, LV_EVENT_PRESSED, NULL);
        lv_obj_add_event_cb(s_voice_btn, _voice_long_press, LV_EVENT_LONG_PRESSED, NULL);
        lv_obj_add_event_cb(s_voice_btn, _voice_pressing, LV_EVENT_PRESSING, NULL);
        lv_obj_add_event_cb(s_voice_btn, _voice_released, LV_EVENT_RELEASED, NULL);
    }

    // 初始化 AI 聊天后端
    app_ai_chat_init();

    // 添加欢迎消息
    if (s_msg_list)
    {
        _add_message("你好！我是AI助手，有什么可以帮你的？", false);
        _scroll_to_bottom();
    }
}

// ============================================================================
// 长按手势处理
// ============================================================================

// 按下时记录起始 Y 坐标（用于上滑位移计算）
static void _voice_pressed(lv_event_t *e)
{
    (void)e;
    lv_indev_t *indev = lv_indev_get_act();
    if (indev)
    {
        lv_point_t p;
        lv_indev_get_point(indev, &p);
        s_press_start_y = p.y;
        EK_LOG_INFO("voice: pressed y=%d", (int)p.y);
    }
    else
    {
        EK_LOG_WARN("voice: pressed but indev=NULL");
    }
}

static void _voice_long_press(lv_event_t *e)
{
    (void)e;
    s_long_pressed = true;
    EK_LOG_INFO("voice: long press triggered");
    _set_voice_state(VOICE_STATE_RECORDING);
}

static void _voice_pressing(lv_event_t *e)
{
    (void)e;
    if (!s_long_pressed)
    {
        return;
    }

    // 检测上滑距离
    lv_indev_t *indev = lv_indev_get_act();
    if (!indev)
    {
        return;
    }
    lv_point_t p;
    lv_indev_get_point(indev, &p);
    lv_coord_t delta_y = p.y - s_press_start_y;

    // 上滑超过阈值 → 取消状态
    if (delta_y < -VOICE_CANCEL_THRESHOLD)
    {
        _set_voice_state(VOICE_STATE_CANCEL);
    }
    else
    {
        _set_voice_state(VOICE_STATE_RECORDING);
    }
}

static void _voice_released(lv_event_t *e)
{
    (void)e;
    if (!s_long_pressed)
    {
        EK_LOG_INFO("voice: released, not long pressed");
        return;
    }
    s_long_pressed = false;

    // 检查从按下到松开的累计上滑距离
    bool cancelled = false;
    lv_indev_t *indev = lv_indev_get_act();
    if (indev)
    {
        lv_point_t p;
        lv_indev_get_point(indev, &p);
        lv_coord_t delta_y = p.y - s_press_start_y;
        EK_LOG_INFO("voice: released y=%d start=%d delta=%d", (int)p.y, (int)s_press_start_y, (int)delta_y);
        if (delta_y < -VOICE_CANCEL_THRESHOLD)
        {
            cancelled = true;
        }
    }
    else
    {
        EK_LOG_WARN("voice: released but indev=NULL");
    }

    _set_voice_state(VOICE_STATE_IDLE);

    // 上滑取消 → 不弹键盘
    if (cancelled)
    {
        EK_LOG_INFO("voice: cancelled");
        return;
    }

    EK_LOG_INFO("voice: released (STT not available)");
    TODO("STT 语音识别未接入，长按松开暂不处理");
}
static void _set_voice_state(voice_state_t state)
{
    s_voice_state = state;
    if (!s_voice_btn || !s_voice_label)
    {
        return;
    }

    switch (state)
    {
    case VOICE_STATE_IDLE:
        lv_obj_set_style_bg_color(s_voice_btn, lv_color_hex(0x3a5066), 0);
        lv_label_set_text(s_voice_label, "按住说话");
        break;
    case VOICE_STATE_RECORDING:
        lv_obj_set_style_bg_color(s_voice_btn, lv_color_hex(0x2a8045), 0);
        lv_label_set_text(s_voice_label, "松开发送  上滑取消");
        break;
    case VOICE_STATE_CANCEL:
        lv_obj_set_style_bg_color(s_voice_btn, lv_color_hex(0x804030), 0);
        lv_label_set_text(s_voice_label, "松开手指  取消");
        break;
    }
}

// ============================================================================
// AI 回复回调（在 app_ai_chat 任务上下文中调用）
// ============================================================================

static void _ai_reply_cb(const char *reply, void *arg)
{
    (void)arg;
    lv_lock();

    // 隐藏 loading
    if (s_loading_label)
    {
        lv_obj_add_flag(s_loading_label, LV_OBJ_FLAG_HIDDEN);
    }

    // 添加 AI 消息到 UI
    if (reply && reply[0])
    {
        _add_message(reply, false);
        _scroll_to_bottom();

        // TTS 暂时不可用，待硬件就绪后替换为 app_tts_say(reply)
        TODO("TTS 播放: %s", reply);
    }

    lv_unlock();
}

// ============================================================================
// 消息气泡
// ============================================================================

static void _add_message(const char *text, bool is_user)
{
    if (!s_msg_list || !text)
    {
        return;
    }

    // 超过上限则删除最旧的消息
    int cnt = lv_obj_get_child_count(s_msg_list);
    if (cnt >= MAX_BUBBLES)
    {
        lv_obj_t *oldest = lv_obj_get_child(s_msg_list, 0);
        if (oldest)
        {
            lv_obj_delete(oldest);
        }
    }

    // wrapper 容器：控制气泡左右对齐
    lv_obj_t *wrapper = lv_obj_create(s_msg_list);
    lv_obj_set_width(wrapper, LV_PCT(100));
    lv_obj_set_height(wrapper, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(wrapper, 0, 0);
    lv_obj_set_style_border_width(wrapper, 0, 0);
    lv_obj_set_style_pad_all(wrapper, 2, 0);
    lv_obj_set_style_layout(wrapper, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(wrapper, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(wrapper,
        is_user ? LV_FLEX_ALIGN_END : LV_FLEX_ALIGN_START, 0);

    // 气泡
    lv_obj_t *bubble = lv_obj_create(wrapper);
    lv_obj_set_width(bubble, LV_PCT(70));
    lv_obj_set_height(bubble, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(bubble,
        is_user ? lv_color_hex(0x3a5066) : lv_color_hex(0x2a4055), 0);
    lv_obj_set_style_bg_opa(bubble, 255, 0);
    lv_obj_set_style_radius(bubble, 12, 0);
    lv_obj_set_style_pad_all(bubble, 8, 0);
    lv_obj_set_style_border_width(bubble, 0, 0);

    // 文字
    lv_obj_t *label = lv_label_create(bubble);
    lv_obj_set_width(label, LV_PCT(100));
    lv_obj_set_height(label, LV_SIZE_CONTENT);
    lv_label_set_text(label, text);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_style_text_font(label, font_chinese_6500_14, 0);
}

static void _scroll_to_bottom(void)
{
    if (!s_msg_list)
    {
        return;
    }
    lv_obj_update_layout(s_msg_list);
    lv_obj_scroll_to_y(s_msg_list, LV_COORD_MAX, LV_ANIM_ON);
}
