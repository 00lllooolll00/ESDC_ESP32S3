#ifndef IMPL_EXIO_H
#define IMPL_EXIO_H

#include "bsp_exio.h"

#ifdef __cplusplus
extern "C"
{
#endif

int impl_exio_init(void);
int impl_exio_int_enable(void);
int impl_exio_int_disable(void);

// 单 pin 操作：薄包 bsp_exio，供 app 层通过 impl 调用（不跳层）
int impl_exio_config_pin(bsp_exio_pin_num_t pin, bsp_exio_pin_mode_t mode);
int impl_exio_write_pin(bsp_exio_pin_num_t pin, uint8_t value);
int impl_exio_read_pin(bsp_exio_pin_num_t pin);

#ifdef __cplusplus
}
#endif

#endif // IMPL_EXIO_H
