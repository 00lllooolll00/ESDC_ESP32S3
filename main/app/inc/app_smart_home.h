#ifndef APP_SMART_HOME_H
#define APP_SMART_HOME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"

// 受控的"没用" XL9555 IO 数量（EXIO9~EXIO15）
#define APP_SMART_HOME_COUNT 7

// 每个按钮名字最大长度（含 '\0'）
#define APP_SMART_HOME_NAME_MAX 16

/**
 * @brief 初始化智能家具 IO 控制（EXIO9-15 配输出+低电平，从 NVS 加载名字/电平）
 *
 * 由 EK_EXPORT_APP 自动调用（level 3，早于 app_ui level 4）。
 * NVS 无数据 / magic 不匹配时用默认名字 + 全低电平，不阻塞启动。
 */
void app_smart_home_init(void);

/**
 * @brief 切换某路 IO 电平并持久化
 *
 * @param idx 索引 0..6，对应 EXIO9..15
 * @return 切换后的电平 0/1；越界返回 -1
 */
int app_smart_home_toggle(int idx);

/**
 * @brief 获取某路 IO 当前电平
 *
 * @param idx 索引 0..6
 * @return 0/1；越界返回 -1
 */
int app_smart_home_get_level(int idx);

/**
 * @brief 获取某路 IO 的名字（只读指针，指向模块内部存储）
 *
 * @param idx 索引 0..6
 * @return 名字串指针；越界返回 "ERR"
 */
const char *app_smart_home_get_name(int idx);

/**
 * @brief 设置某路 IO 的名字并持久化（超长截断到 NAME_MAX-1）
 *
 * @param idx 索引 0..6
 * @param name 名字串（NULL 视为清空）
 */
void app_smart_home_set_name(int idx, const char *name);

/**
 * @brief 恢复默认：名字重置为 EXIO9..EXIO15、电平全低、写回 NVS
 */
void app_smart_home_reset_default(void);

#ifdef __cplusplus
}
#endif

#endif // APP_SMART_HOME_H
