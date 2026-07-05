#include "common_header.h"

#include "app_smart_home.h"
#include "impl_exio.h"
#include "ek_export.h"
#include "nvs.h"
#include <string.h>

EK_LOG_FILE_TAG("app_smart_home");

#define APP_SMART_HOME_NVS_NS  "ek_shome"
#define APP_SMART_HOME_NVS_KEY "io_cfg"
#define APP_SMART_HOME_MAGIC   0x53484F4E // "SHON"（bump 以丢弃旧版默认名）

// 持久化结构：magic + 7 路 {名字, 电平}，整体作为 NVS blob 存取
typedef struct
{
    uint32_t magic;
    char names[APP_SMART_HOME_COUNT][APP_SMART_HOME_NAME_MAX];
    uint8_t levels[APP_SMART_HOME_COUNT];
} app_smart_home_store_t;

// idx 0..6 ↔ EXIO9..15（硬件引脚不变，仅显示名重新编号为 IO1~IO7）
static const bsp_exio_pin_num_t s_pins[APP_SMART_HOME_COUNT] = {
    BSP_EXIO_NUM9, BSP_EXIO_NUM10, BSP_EXIO_NUM11, BSP_EXIO_NUM12, BSP_EXIO_NUM13, BSP_EXIO_NUM14, BSP_EXIO_NUM15,
};

static const char *s_default_names[APP_SMART_HOME_COUNT] = {
    "IO1", "IO2", "IO3", "IO4", "IO5", "IO6", "IO7",
};

static app_smart_home_store_t s_store;

// 用默认名字 + 全低电平填充内存 store（不写 NVS）
static void _fill_defaults(void)
{
    s_store.magic = APP_SMART_HOME_MAGIC;
    for (int i = 0; i < APP_SMART_HOME_COUNT; i++)
    {
        strlcpy(s_store.names[i], s_default_names[i], APP_SMART_HOME_NAME_MAX);
        s_store.levels[i] = 0;
    }
}

// 从 NVS 加载；失败 / 无数据 / magic 不符则用默认值填充
static void _load(void)
{
    nvs_handle_t h;
    if (nvs_open(APP_SMART_HOME_NVS_NS, NVS_READONLY, &h) != ESP_OK)
    {
        EK_LOG_INFO("smart home store: no data, start default");
        _fill_defaults();
        return;
    }

    size_t required = sizeof(s_store);
    esp_err_t err = nvs_get_blob(h, APP_SMART_HOME_NVS_KEY, &s_store, &required);
    nvs_close(h);

    if (err != ESP_OK || required != sizeof(s_store) || s_store.magic != APP_SMART_HOME_MAGIC)
    {
        EK_LOG_WARN("smart home store: invalid data, start default: %s", unified_strerror(err));
        _fill_defaults();
        return;
    }
}

// 写回 NVS（在 UI 线程 lv_lock 内调用，NVS 写入几十 ms 偶发可接受）
static void _persist(void)
{
    nvs_handle_t h;
    if (nvs_open(APP_SMART_HOME_NVS_NS, NVS_READWRITE, &h) != ESP_OK)
    {
        EK_LOG_WARN("smart home store: nvs open failed");
        return;
    }
    esp_err_t err = nvs_set_blob(h, APP_SMART_HOME_NVS_KEY, &s_store, sizeof(s_store));
    if (err != ESP_OK)
    {
        EK_LOG_WARN("smart home store: nvs set_blob failed: %s", unified_strerror(err));
    }
    else
    {
        nvs_commit(h);
    }
    nvs_close(h);
}

void app_smart_home_init(void)
{
    EK_LOG_INFO("ek_export: APP app_smart_home_init");

    // 先把 7 路 IO 配成输出 + 低电平（硬件安全初始态）
    for (int i = 0; i < APP_SMART_HOME_COUNT; i++)
    {
        impl_exio_config_pin(s_pins[i], BSP_EXIO_PIN_MODE_OUTPUT);
        impl_exio_write_pin(s_pins[i], 0);
    }

    // 从 NVS 加载名字 / 电平；失败用默认值
    _load();

    // 把持久化的电平同步到硬件（加载成功时 levels 可能非 0）
    for (int i = 0; i < APP_SMART_HOME_COUNT; i++)
    {
        impl_exio_write_pin(s_pins[i], s_store.levels[i]);
    }
}

EK_EXPORT_APP(app_smart_home_init, 3);

int app_smart_home_toggle(int idx)
{
    if (idx < 0 || idx >= APP_SMART_HOME_COUNT) return -1;
    s_store.levels[idx] ^= 1;
    impl_exio_write_pin(s_pins[idx], s_store.levels[idx]);
    _persist();
    return s_store.levels[idx];
}

int app_smart_home_get_level(int idx)
{
    if (idx < 0 || idx >= APP_SMART_HOME_COUNT) return -1;
    return s_store.levels[idx];
}

const char *app_smart_home_get_name(int idx)
{
    if (idx < 0 || idx >= APP_SMART_HOME_COUNT) return "ERR";
    return s_store.names[idx];
}

void app_smart_home_set_name(int idx, const char *name)
{
    if (idx < 0 || idx >= APP_SMART_HOME_COUNT) return;
    if (name == NULL) name = "";
    strlcpy(s_store.names[idx], name, APP_SMART_HOME_NAME_MAX);
    _persist();
}

void app_smart_home_reset_default(void)
{
    _fill_defaults();
    for (int i = 0; i < APP_SMART_HOME_COUNT; i++)
    {
        impl_exio_write_pin(s_pins[i], 0);
    }
    _persist();
    EK_LOG_INFO("smart home store: reset to default");
}
