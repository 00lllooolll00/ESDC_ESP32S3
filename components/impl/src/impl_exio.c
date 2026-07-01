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
