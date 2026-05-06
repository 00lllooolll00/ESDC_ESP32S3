#ifndef BSP_TOUCH_H
#define BSP_TOUCH_H

#include "common_header.h"
#include "bsp_exio.h"

#define BSP_TOUCH_TIMEOUT   (100)
#define BSP_TOUCH_INT_STACK (4 * 1024)
#define BSP_TOUCH_POINT_NUM (5)

/* 触摸芯片引脚相关定义 */
#define BSP_TOUCH_INT_PIN   GPIO_NUM_40
#define BSP_TOUCH_RST_PIN   BSP_EXIO_NUM9

#define BSP_TOUCH_INT_GET() gpio_get_level(BSP_TOUCH_INT_PIN)
#define BSP_TOUCH_CT_RST(x) bsp_exio_write_pin(BSP_TOUCH_RST_PIN, x)

/* GT器件地址及IIC读写命令 */
#define BSP_TOUCH_DEV_ADDR (0x14) /* GTXXX设备地址 */
#define BSP_TOUCH_CMD_WR   (0X28) /* 写命令 */
#define BSP_TOUCH_CMD_RD   (0X29) /* 读命令 */

/* GT9XXX 部分寄存器定义  */
#define BSP_TOUCH_CTRL_REG  (0X8040) /* GT9XXX控制寄存器 */
#define BSP_TOUCH_CFGS_REG  (0X8047) /* GT9XXX配置起始地址寄存器 */
#define BSP_TOUCH_CHECK_REG (0X80FF) /* GT9XXX校验和寄存器 */
#define BSP_TOUCH_PID_REG   (0X8140) /* GT9XXX产品ID寄存器 */

#define BSP_TOUCH_GSTID_REG (0X814E) /* GT9XXX当前检测到的触摸情况 */
#define BSP_TOUCH_TP1_REG   (0X814F) /* 第一个触摸点数据地址 */
#define BSP_TOUCH_TP2_REG   (0X8158) /* 第二个触摸点数据地址 */
#define BSP_TOUCH_TP3_REG   (0X8160) /* 第三个触摸点数据地址 */
#define BSP_TOUCH_TP4_REG   (0X8168) /* 第四个触摸点数据地址 */
#define BSP_TOUCH_TP5_REG   (0X8170) /* 第五个触摸点数据地址 */
#define BSP_TOUCH_TP6_REG   (0X8178) /* 第六个触摸点数据地址 */
#define BSP_TOUCH_TP7_REG   (0X8180) /* 第七个触摸点数据地址 */
#define BSP_TOUCH_TP8_REG   (0X8188) /* 第八个触摸点数据地址 */
#define BSP_TOUCH_TP9_REG   (0X8190) /* 第九个触摸点数据地址 */
#define BSP_TOUCH_TP10_REG  (0X8198) /* 第十个触摸点数据地址 */

typedef struct
{
    uint8_t id;
    uint16_t x;
    uint16_t y;
} bsp_touch_pos_t;

typedef struct
{
    uint8_t count;
    bsp_touch_pos_t points[BSP_TOUCH_POINT_NUM];
} bsp_touch_points_t;

void bsp_touch_init(void);
void bsp_touch_register_int_cb(void (*cb)(void));
void bsp_touch_int_enable(void);
void bsp_touch_int_disable(void);
esp_err_t bsp_touch_read(bsp_touch_points_t *points);

#endif // BSP_TOUCH_H
