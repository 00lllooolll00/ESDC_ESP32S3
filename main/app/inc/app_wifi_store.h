#ifndef APP_WIFI_STORE_H
#define APP_WIFI_STORE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"

#define APP_WIFI_STORE_MAX 5

// 一组 WiFi 凭据（SSID + 密码）
typedef struct
{
    char ssid[33];
    char password[64];
} wifi_cred_t;

/**
 * @brief 初始化 WiFi 凭据存储（从 NVS 加载已存记录）
 *
 * 由 EK_EXPORT_APP 自动调用，需在 UI 初始化前执行
 */
void app_wifi_store_init(void);

/**
 * @brief 保存一组凭据（连接成功后调用）
 *
 * 同 SSID 已存在则更新密码；否则追加；满 APP_WIFI_STORE_MAX 时淘汰最早的一条。
 * 变更后同步写回 NVS。
 *
 * @param ssid SSID
 * @param password 密码
 */
void app_wifi_store_add(const char *ssid, const char *password);

/**
 * @brief 删除一组凭据（“忘记密码”）
 *
 * @param ssid 要删除的 SSID
 * @return true 删除成功
 * @return false 未找到该 SSID
 */
bool app_wifi_store_remove(const char *ssid);

/**
 * @brief 查找已存凭据
 *
 * @param ssid 要查找的 SSID
 * @param password_out 找到时拷贝密码；不需要时可传 NULL
 * @param len password_out 容量
 * @return true 找到
 * @return false 未找到
 */
bool app_wifi_store_find(const char *ssid, char *password_out, size_t len);

/**
 * @brief 获取凭据数组只读指针和数量
 *
 * @param count_out 输出已存凭据条数
 * @return 凭据数组指针（模块内部存储，调用者不可修改）
 */
const wifi_cred_t *app_wifi_store_get_all(int *count_out);

#ifdef __cplusplus
}
#endif

#endif // APP_WIFI_STORE_H
