#ifndef APP_IP_LOCATION_H
#define APP_IP_LOCATION_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"

/**
 * @brief 城市名更新时回调，在 lv_lock 保护下调用
 *
 * @param city 当前城市名（UTF-8，模块内静态缓冲，回调期间有效）
 * @param arg 透传给回调的用户参数
 */
typedef void (*app_ip_location_ui_cb_t)(const char *city, void *arg);

/**
 * @brief 初始化 IP 定位模块（建互斥锁 + 建拉取 task）
 *
 * 由 EK_EXPORT_APP 自动调用，无需手动调用
 */
void app_ip_location_init(void);

/**
 * @brief 非阻塞触发一次 ip-api 城市拉取
 *
 * 可在持 lv_lock 的上下文调用；实际 HTTP 在独立 task 内阻塞执行，
 * 成功后通过 register_ui_cb 注册的回调（lv_lock 保护下）通知 UI
 */
void app_ip_location_request(void);

/**
 * @brief 写入城市名并触发 UI 回调
 *
 * @param city 城市名（UTF-8），内部拷贝；NULL 视为清空
 *
 * 线程安全：内部互斥锁保护存储；触发 UI 回调时用 lv_lock/lv_unlock 包裹
 */
void app_ip_location_set_city(const char *city);

/**
 * @brief 获取当前城市名快照（线程安全拷贝）
 *
 * @param buf 输出缓冲
 * @param len 缓冲容量；无城市时写入空串
 */
void app_ip_location_get_city(char *buf, size_t len);

/**
 * @brief 注册 UI 回调，城市名更新时在 lv_lock 保护下调用
 *
 * @param cb 回调函数指针
 * @param arg 透传给回调的用户参数
 */
void app_ip_location_register_ui_cb(app_ip_location_ui_cb_t cb, void *arg);

#ifdef __cplusplus
}
#endif

#endif // APP_IP_LOCATION_H
