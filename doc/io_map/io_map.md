# ESP32-S3 IO 引脚分布图

> 基于 `io_map_new.md`（新版 PCB 引脚分配）。

---

## 1. LCD 显示模块（经 FPC1 连接器）

| MCU引脚号 | MCU引脚名 | 网络名 | 功能 | FPC1引脚号 |
|---|---|---|---|---|
| 4 | IO4 | LCD_DE | 数据使能 | 8 |
| 5 | IO5 | LCD_CLK | 像素时钟 | 11 |
| 6 | IO6 | LCD_B7 | 蓝色 Bit7 | 13 |
| 7 | IO7 | LCD_B6 | 蓝色 Bit6 | 14 |
| 8 | IO15 | LCD_B5 | 蓝色 Bit5 | 15 |
| 9 | IO16 | LCD_B4 | 蓝色 Bit4 | 16 |
| 10 | IO17 | LCD_B3 | 蓝色 Bit3 | 17 |
| 11 | IO18 | LCD_G7 | 绿色 Bit7 | 22 |
| 12 | IO8 | LCD_G6 | 绿色 Bit6 | 23 |
| 14 | IO20 | LCD_BL | 背光控制 | 7 |
| 15 | IO3 | LCD_G5 | 绿色 Bit5 | 24 |
| 16 | IO46 | LCD_G4 | 绿色 Bit4 | 25 |
| 17 | IO9 | LCD_G3 | 绿色 Bit3 | 26 |
| 18 | IO10 | LCD_G2 | 绿色 Bit2 | 27 |
| 19 | IO11 | LCD_R7 | 红色 Bit7 | 31 |
| 20 | IO12 | LCD_R6 | 红色 Bit6 | 32 |
| 21 | IO13 | LCD_R5 | 红色 Bit5 | 33 |
| 22 | IO14 | LCD_R4 | 红色 Bit4 | 34 |
| 23 | IO21 | LCD_R3 | 红色 Bit3 | 35 |

---

## 2. 触摸控制器（经 FPC1 连接器）

| MCU引脚号 | MCU引脚名 | 网络名 | 功能 | FPC1引脚号 |
|---|---|---|---|---|
| 31 | IO38 | CT_SDA | 触摸I2C数据 | 5 |
| 32 | IO39 | CT_SCL | 触摸I2C时钟 | 3 |
| 33 | IO40 | CT_INT | 触摸中断输出 | 2 |

> 触摸复位 CT_RST 由 XL9555 的 IO0_4（Pin8）输出，经 FPC1 Pin6 连接触摸芯片。

---

## 3. I2C 总线（XL9555 + ES8388 共用）

| MCU引脚号 | MCU引脚名 | 网络名 | 功能 | 连接目标 |
|---|---|---|---|---|
| 34 | IO41 | I2C0_SCL | I2C时钟 | U2 Pin22 (XL9555 SCL), U6 Pin28 (ES8388 CCLK) |
| 35 | IO42 | I2C0_SDA | I2C数据 | U2 Pin23 (XL9555 SDA), U6 Pin27 (ES8388 CDATA) |

---

## 4. XL9555 IO 扩展模块（U2）

| MCU引脚号 | MCU引脚名 | 网络名 | 功能 |
|---|---|---|---|
| 13 | IO19 | XL_INT | XL9555中断输出到MCU |
| 34 | IO41 | I2C0_SCL | I2C时钟（共用总线） |
| 35 | IO42 | I2C0_SDA | I2C数据（共用总线） |

**XL9555 扩展 IO 分配表：**

| XL9555引脚 | 网络名 | 连接目标 |
|---|---|---|
| IO0_0 (Pin4) | EXIO0 | R23(330R) 驱动 U8 RGB-LED 红色 |
| IO0_1 (Pin5) | EXIO1 | R24(100R) 驱动 U8 RGB-LED 绿色 |
| IO0_2 (Pin6) | EXIO2 | R25(100R) 驱动 U8 RGB-LED 蓝色 |
| IO0_3 (Pin7) | EXIO3 | U7 (MD8002A) SHUTDOWN 引脚 |
| IO0_4 (Pin8) | CT_RST | FPC1 Pin6 连接触摸芯片复位 |
| IO0_5 (Pin9) | EXIO5 | KEY1 按键 |
| IO0_6 (Pin10) | EXIO6 | KEY2 按键 |
| IO0_7 (Pin11) | EXIO7 | KEY3 按键 |
| IO1_0 (Pin13) | EXIO8 | H1 排针 Pin2 |
| IO1_1 (Pin14) | EXIO9 | H1 排针 Pin1 |
| IO1_2 (Pin15) | EXIO10 | H1 排针 Pin4 |
| IO1_3 (Pin16) | EXIO11 | H1 排针 Pin3 |
| IO1_4 (Pin17) | EXIO12 | H1 排针 Pin6 |
| IO1_5 (Pin18) | EXIO13 | H1 排针 Pin5 |
| IO1_6 (Pin19) | EXIO14 | H1 排针 Pin8 |
| IO1_7 (Pin20) | EXIO15 | H1 排针 Pin7 |

---

## 5. I2S 音频模块（ES8388 编解码器 U6）

| MCU引脚号 | MCU引脚名 | 网络名 | 功能 | ES8388对应引脚 |
|---|---|---|---|---|
| 24 | IO47 | I2S_SDOUT | 数据输出(MCU到ES8388) | Pin6 (DSDIN), 经R16(22R)串联 |
| 25 | IO48 | I2S_LRCK | 左右声道时钟 | Pin7 (LRCK), 经R15(22R)串联 |
| 26 | IO45 | I2S_SDIN | 数据输入(ES8388到MCU) | Pin8 (ASDOUT), 经R14(22R)串联 |
| 28 | IO35 | I2S_SCK | 位时钟(BCLK) | Pin5 (SCLK), 经R13(22R)串联 |
| 29 | IO36 | I2S_MCLK | 主时钟 | Pin1 (MCLK) |

**音频信号链路：**
- 扬声器输出：ES8388 LOUT2/ROUT2 (Pin15/14) 经 C18/C19(1uF) 耦合到 SPK_IN，再经 R18(5.6K) 到 U7 (MD8002A)，驱动 SPK1 扬声器
- 麦克风输入：MIC1 经 C32/C33(100nF) 耦合到 MIC_P/MIC_N，送入 ES8388 LIN1/RIN1 (Pin24/23)
- 功放控制：XL9555 EXIO3 控制 U7 (MD8002A) SHUTDOWN 引脚

---

## 6. USB 转串口模块（CH340C U3 + Type-C USB2）

| MCU引脚号 | MCU引脚名 | 网络名 | 功能 | CH340C对应引脚 |
|---|---|---|---|---|
| 36 | RXD0 | RXD0 | MCU串口接收 | Pin2 (TXD) |
| 37 | TXD0 | TXD0 | MCU串口发送 | Pin3 (RXD) |

**自动下载电路（Q1/Q2 + R6/R7）：**
- CH340C RTS# (Pin14) 经 R6(10K) 到 Q2 基极，Q2 集电极控制 RESET
- CH340C DTR# (Pin13) 经 R7(10K) 到 Q1 基极，Q1 集电极控制 BOOT

**USB信号：** CH340C D+/D- (Pin5/6) 连接 Type-C (USB2) 的 D+/D-

---

## 7. BOOT / RESET 按键模块

| MCU引脚号 | MCU引脚名 | 网络名 | 功能 | 按键 |
|---|---|---|---|---|
| 3 | EN | RESET | 复位引脚 | RESET按键(另一端GND) |
| 27 | IO0 | BOOT | 启动模式选择 | BOOT按键(另一端GND) |

> 复位电路：R1(10K) 上拉至 VCC + C3(100nF) 滤波

---

## 8. 电源模块

| MCU引脚号 | MCU引脚名 | 网络名 | 功能 |
|---|---|---|---|
| 2 | 3V3 | VCC | 3.3V主电源(U4 RT9013-33GB输出) |
| 1 | GND | GND | 数字地 |
| 40 | GND | GND | 数字地 |
| 41 | GND | GND | 数字地 |

**电源链路：** USB Type-C VBUS 经 SW3开关 和 F1(自恢复保险丝) 到 +5V，再分别经 U4 (RT9013-33GB) 输出 VCC(3.3V数字电源) 和 U5 (RT9013-33GB) 输出 VCC_A(3.3V模拟电源供ES8388)

---

## 9. 未使用引脚

| MCU引脚号 | MCU引脚名 | 网络名 | 状态 |
|---|---|---|---|
| 30 | IO37 | (空) | 未连接 |
| 38 | IO2 | (空) | 未连接 |
| 39 | IO1 | (空) | 未连接 |

---
