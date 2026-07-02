/**
 * @file ui.h
 */

#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "ui_gen.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

// 中文字体（从 VFS 加载）
extern lv_font_t *g_ui_font_chinese_3500_14;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

// 初始化（由 app_ui.c 调用）
void ui_init(const char *asset_path);

#ifdef __cplusplus
}
#endif

#endif /*UI_H*/
