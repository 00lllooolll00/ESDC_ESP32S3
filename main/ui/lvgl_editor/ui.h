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


/**********************
 * GLOBAL PROTOTYPES
 **********************/

// 初始化（由 app_ui.c 调用）
void ui_init(const char *asset_path);

#ifdef __cplusplus
}
#endif

#endif /*UI_H*/
