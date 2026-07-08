#ifndef APP_AI_CHAT_H
#define APP_AI_CHAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common_header.h"

#define APP_AI_CHAT_MAX_HISTORY  10   // 最多保存 10 条历史消息（5 轮对话）
#define APP_AI_CHAT_MAX_TEXT     256  // 单条消息最大长度
#define APP_AI_CHAT_MAX_REPLY    512  // AI 回复最大长度

typedef void (*app_ai_chat_reply_cb)(const char *reply, void *arg);

// 初始化队列（由 ai_chat_actions_init 调用，不用 EK_EXPORT_APP）
void app_ai_chat_init(void);

// 发送用户消息（异步，AI 回复通过 cb 返回，cb 在 app_ai_chat 任务上下文中调用）
void app_ai_chat_send(const char *text, app_ai_chat_reply_cb cb, void *arg);

// 清空对话历史
void app_ai_chat_clear_history(void);

#ifdef __cplusplus
}
#endif

#endif // APP_AI_CHAT_H
