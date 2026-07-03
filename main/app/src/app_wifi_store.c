#include "common_header.h"

#include "app_wifi_store.h"
#include "ek_export.h"
#include "nvs.h"
#include <string.h>

EK_LOG_FILE_TAG("app_wifi_store");

#define WIFI_STORE_NVS_NS  "ek_wifi"
#define WIFI_STORE_NVS_KEY "cred"

// 持久化结构：count + 凭据数组，整体作为 NVS blob 存取
typedef struct
{
    int count;
    wifi_cred_t items[APP_WIFI_STORE_MAX];
} store_persist_t;

static store_persist_t s_store;

// 写回 NVS（在 lv_lock 内调用，NVS 写入耗时几十 ms，偶发可接受）
static void _persist(void)
{
    nvs_handle_t h;
    if (nvs_open(WIFI_STORE_NVS_NS, NVS_READWRITE, &h) != ESP_OK)
    {
        EK_LOG_WARN("nvs open failed");
        return;
    }
    esp_err_t err = nvs_set_blob(h, WIFI_STORE_NVS_KEY, &s_store, sizeof(s_store));
    if (err != ESP_OK)
    {
        EK_LOG_WARN("nvs set_blob failed: %s", unified_strerror(err));
    }
    else
    {
        nvs_commit(h);
    }
    nvs_close(h);
}

void app_wifi_store_init(void)
{
    EK_LOG_INFO("ek_export: APP app_wifi_store_init");
    memset(&s_store, 0, sizeof(s_store));

    nvs_handle_t h;
    if (nvs_open(WIFI_STORE_NVS_NS, NVS_READONLY, &h) != ESP_OK)
    {
        EK_LOG_INFO("nvs open readonly failed, start empty");
        return;
    }
    size_t required = sizeof(s_store);
    esp_err_t err = nvs_get_blob(h, WIFI_STORE_NVS_KEY, &s_store, &required);
    if (err != ESP_OK || required != sizeof(s_store) || s_store.count < 0 || s_store.count > APP_WIFI_STORE_MAX)
    {
        EK_LOG_WARN("nvs load invalid, start empty: %s", unified_strerror(err));
        memset(&s_store, 0, sizeof(s_store));
    }
    else
    {
        EK_LOG_INFO("loaded %d saved wifi", s_store.count);
    }
    nvs_close(h);
}

// 在 UI 初始化前加载，供 wifi_actions 查询
EK_EXPORT_APP(app_wifi_store_init, 1);

void app_wifi_store_add(const char *ssid, const char *password)
{
    if (!ssid || !password)
    {
        return;
    }

    // 已存在同 SSID → 更新密码
    for (int i = 0; i < s_store.count; i++)
    {
        if (strcmp(s_store.items[i].ssid, ssid) == 0)
        {
            strlcpy(s_store.items[i].password, password, sizeof(s_store.items[i].password));
            _persist();
            EK_LOG_INFO("updated password for %s", ssid);
            return;
        }
    }

    // 不存在：满则整体前移淘汰最早的一条
    if (s_store.count >= APP_WIFI_STORE_MAX)
    {
        memmove(&s_store.items[0], &s_store.items[1],
                (APP_WIFI_STORE_MAX - 1) * sizeof(wifi_cred_t));
        s_store.count = APP_WIFI_STORE_MAX - 1;
        EK_LOG_INFO("store full, evicted oldest");
    }

    // 追加到尾部
    strlcpy(s_store.items[s_store.count].ssid, ssid, sizeof(s_store.items[0].ssid));
    strlcpy(s_store.items[s_store.count].password, password, sizeof(s_store.items[0].password));
    s_store.count++;
    _persist();
    EK_LOG_INFO("added %s, count=%d", ssid, s_store.count);
}

bool app_wifi_store_remove(const char *ssid)
{
    if (!ssid)
    {
        return false;
    }
    for (int i = 0; i < s_store.count; i++)
    {
        if (strcmp(s_store.items[i].ssid, ssid) == 0)
        {
            // 后移覆盖被删项
            memmove(&s_store.items[i], &s_store.items[i + 1],
                    (s_store.count - i - 1) * sizeof(wifi_cred_t));
            s_store.count--;
            _persist();
            EK_LOG_INFO("removed %s, count=%d", ssid, s_store.count);
            return true;
        }
    }
    return false;
}

bool app_wifi_store_find(const char *ssid, char *password_out, size_t len)
{
    if (!ssid)
    {
        return false;
    }
    for (int i = 0; i < s_store.count; i++)
    {
        if (strcmp(s_store.items[i].ssid, ssid) == 0)
        {
            if (password_out && len > 0)
            {
                strlcpy(password_out, s_store.items[i].password, len);
            }
            return true;
        }
    }
    return false;
}
