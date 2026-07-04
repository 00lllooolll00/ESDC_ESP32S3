#include "impl_exio.h"
#include "bsp_exio.h"
#include "ek_export.h"

EK_LOG_FILE_TAG("impl_exio.c");

// exio 初始化：最底层 IO 扩展，所有设备 register 的前提
int impl_exio_init(void)
{
    bsp_exio_init();
    return 0;
}

// 自动注册入口（无参适配 EK_EXPORT）
static void impl_exio_auto_init(void)
{
    EK_LOG_INFO("ek_export: HARDWARE impl_exio_auto_init");
    (void)impl_exio_init();
}

EK_EXPORT_HARDWARE(impl_exio_auto_init, 0);

int impl_exio_int_enable(void)
{
    bsp_exio_int_enable();
    return 0;
}

int impl_exio_int_disable(void)
{
    bsp_exio_int_disable();
    return 0;
}

// 配置单个 XL9555 GPIO 方向（薄包 bsp_exio_conifg_pin，注意 bsp 拼写）
int impl_exio_config_pin(bsp_exio_pin_num_t pin, bsp_exio_pin_mode_t mode)
{
    bsp_exio_pin_config_t cfg = { .pin = pin, .mode = mode };
    bsp_exio_conifg_pin(&cfg);
    return 0;
}

// 设置单个 XL9555 GPIO 输出电平
int impl_exio_write_pin(bsp_exio_pin_num_t pin, uint8_t value)
{
    bsp_exio_write_pin(pin, value);
    return 0;
}

// 读取单个 XL9555 GPIO 输入电平，返回 0/1
int impl_exio_read_pin(bsp_exio_pin_num_t pin)
{
    return (int)bsp_exio_read_pin(pin);
}
