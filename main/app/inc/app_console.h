#ifndef APP_CONSOLE_H
#define APP_CONSOLE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"

/**
 * @brief 初始化串口命令台
 *
 * 初始化 esp_console + linenoise，注册测试命令，起 REPL task
 */
void app_console_init(void);

#ifdef __cplusplus
}
#endif

#endif // APP_CONSOLE_H
