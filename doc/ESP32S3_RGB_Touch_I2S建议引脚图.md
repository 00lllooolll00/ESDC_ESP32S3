# ESP32-S3 新板最终连接清单

## 1. 约束

| 项目 | 结论 |
|---|---|
| 屏幕 | 使用与当前兼容的正点原子 `4.3` 寸 `RGBLCD` 电容触摸模块 |
| 音频 Codec | 使用与当前 `DNESP32S3` 相同的 `ES8388` |
| 语音 | 需要录音 + 播放 |
| 串口下载/日志 | 必须保留 `UART0`，并通过 `Type-C` 口实现烧录、下载、查看日志 |
| 其它外设 | 不要 `TF`、不要 `红外`、不要 `DS18B20`、保留 `LED` |
| IO 扩展 | 允许使用 `XL9555`，但仅用于慢速控制 |

## 1.1 候选 IO 查证结论

| IO | 计划用途 | 查证结论 | 最终采用 | 约束 |
|---|---|---|---|---|
| `IO43` | `UART0 TX` | `UART0` 默认脚，可做串口下载/日志 | 是 | 固定给 `UART0` |
| `IO44` | `UART0 RX` | `UART0` 默认脚，可做串口下载/日志 | 是 | 固定给 `UART0` |
| `IO0` | `BOOT/自动下载` | 启动相关脚，适合 `BOOT + 自动下载` | 是 | 不再挂其它外设 |
| `IO19` | `XL9555 INT` | 原生 `USB D-`，但官方允许配置成普通 GPIO，GPIO 中断可用 | 是 | 前提是**不使用原生 USB** |
| `IO20` | `LCD BL_PWM` | 原生 `USB D+`，但官方允许配置成普通 GPIO，`LEDC` 可输出 PWM | 是 | 前提是**不使用原生 USB** |

| 总原则 | 结论 |
|---|---|
| `Type-C` 数据路径 | 走 `Type-C -> USB-UART -> UART0(IO43/44)` |
| 原生 USB | **不使用** |
| `IO19/20` 可用前提 | 只有在不使用原生 `USB D-/D+` 时，才能复用为普通 GPIO |

---

## 2. Type-C 串口烧录模块

| 子模块 | 信号 | 连接到 | ESP32-S3 IO / 引脚 | 备注 |
|---|---|---|---|---|
| `Type-C` | `VBUS` | `5V 输入` | - | 作为整板主输入之一 |
| `Type-C` | `GND` | `系统地` | - | 共地 |
| `USB-UART 芯片` | `TXD` | `ESP32 U0RXD` | `IO44` | 串口下载/日志输入 |
| `USB-UART 芯片` | `RXD` | `ESP32 U0TXD` | `IO43` | 串口下载/日志输出 |
| `USB-UART 芯片` | `DTR` | `GPIO0 自动下载链路` | `IO0` | 用于进入下载模式 |
| `USB-UART 芯片` | `RTS` | `EN 自动复位链路` | `EN / CHIP_PU` | 用于自动复位 |
| `BOOT 按键` | 一端接地 | 一端接 `GPIO0` | `IO0` | 手动进入下载模式 |
| `RESET 按键` | 一端接地 | 一端接 `EN` | `EN / CHIP_PU` | 手动复位 |

| `Type-C` 数据线 | `D+ / D-` | 接 `USB-UART` 芯片 USB 侧 | 不接 `ESP32 IO19/20` | 本板采用 `UART0` 方案，不走原生 USB |

### 说明

| 项目 | 结论 |
|---|---|
| `UART0` 固定脚 | `U0TXD = IO43`，`U0RXD = IO44` |
| `GPIO0` 用途 | 只保留给 `BOOT/自动下载` |
| `EN` 用途 | 只保留给复位链路 |
| `XL9555 INT` | 改接独立 GPIO，中断脚不再占 `IO0` |

---

## 3. RGB 屏幕模块

| 子模块 | 信号 | ESP32-S3 IO / EXIO | 备注 |
|---|---|---|---|
| `RGB LCD` | `DE` | `IO4` | 保持兼容当前工程 |
| `RGB LCD` | `PCLK` | `IO5` | 保持兼容当前工程 |
| `RGB LCD` | `B3` | `IO17` | 保持兼容当前工程 |
| `RGB LCD` | `B4` | `IO16` | 保持兼容当前工程 |
| `RGB LCD` | `B5` | `IO15` | 保持兼容当前工程 |
| `RGB LCD` | `B6` | `IO7` | 保持兼容当前工程 |
| `RGB LCD` | `B7` | `IO6` | 保持兼容当前工程 |
| `RGB LCD` | `G2` | `IO10` | 保持兼容当前工程 |
| `RGB LCD` | `G3` | `IO9` | 保持兼容当前工程 |
| `RGB LCD` | `G4` | `IO46` | ⚠️ Strapping 引脚（启动模式），复位时必须为高电平，建议外部 10kΩ 上拉到 3.3V |
| `RGB LCD` | `G5` | `IO3` | ⚠️ Strapping 引脚（JTAG 信号源选择），复位时必须为高电平，建议外部 10kΩ 上拉到 3.3V |
| `RGB LCD` | `G6` | `IO8` | 保持兼容当前工程 |
| `RGB LCD` | `G7` | `IO18` | 保持兼容当前工程 |
| `RGB LCD` | `R3` | `IO45` | ⚠️ Strapping 引脚（VDD_SPI 电压：低→1.8V / 高→3.3V），被拉低会导致 Flash 无法工作，必须外部 10kΩ 上拉到 3.3V |
| `RGB LCD` | `R4` | `IO48` | 保持兼容当前工程 |
| `RGB LCD` | `R5` | `IO47` | 保持兼容当前工程 |
| `RGB LCD` | `R6` | `IO21` | 保持兼容当前工程 |
| `RGB LCD` | `R7` | `IO14` | 保持兼容当前工程 |
| `RGB LCD` | `BL_EN` | `EXIO_NUM8` | 背光总使能 |
| `RGB LCD` | `BL_PWM` | `IO20` | 背光亮度 PWM 调节；该脚是原生 `USB D+`，前提是不使用原生 USB |
| `RGB LCD` | `5V` | `5V_LCD` | 屏幕模组与背光供电 |
| `RGB LCD` | `GND` | `GND` | 共地 |

---

## 4. 触摸模块

| 子模块 | 信号 | ESP32-S3 IO / EXIO | 备注 |
|---|---|---|---|
| `Touch` | `I2C1_SCL` | `IO38` | 保持兼容当前工程 |
| `Touch` | `I2C1_SDA` | `IO39` | 保持兼容当前工程 |
| `Touch` | `INT` | `IO40` | 触摸中断 |
| `Touch` | `RST` | `EXIO_NUM9` | 触摸复位 |
| `Touch` | `VCC` | `3V3_SYS` | 触摸逻辑供电 |
| `Touch` | `GND` | `GND` | 共地 |

---

## 5. 音频 Codec 控制模块

| 子模块 | 信号 | ESP32-S3 IO | 备注 |
|---|---|---|---|
| `ES8388` | `I2C0_SCL` | `IO42` | Codec 控制总线 |
| `ES8388` | `I2C0_SDA` | `IO41` | Codec 控制总线 |
| `ES8388` | `DVDD/AV视频电源` | `3V3_SYS` | 具体去耦按器件手册 |
| `ES8388` | `GND/AGND` | `GND/AGND` | 模拟地与数字地处理注意回流 |

---

## 6. I2S 语音模块

| 子模块 | 信号 | ESP32-S3 IO | 备注 |
|---|---|---|---|
| `I2S` | `MCLK` | `IO11` | 音频主时钟 |
| `I2S` | `BCLK` | `IO12` | 位时钟 |
| `I2S` | `LRCK/WS` | `IO13` | 声道时钟 |
| `I2S` | `DOUT` | `IO1` | ESP32 → ES8388，播放数据 |
| `I2S` | `DIN` | `IO2` | ES8388 → ESP32，录音数据 |

---

## 7. 功放与喇叭模块

| 子模块 | 信号 | ESP32-S3 IO / EXIO / 电源 | 备注 |
|---|---|---|---|
| `功放` | `PA_EN` | `EXIO_NUM2` | 功放使能 |
| `功放` | `5V` | `5V_AMP` | 建议独立 5V 支路 |
| `功放` | `GND` | `GND` | 共地，注意大电流回路 |
| `ES8388` | `模拟输出` | 接功放输入 | 不占额外 ESP32 GPIO |
| `喇叭` | `SPK+/-` | 接功放输出 | 不占额外 ESP32 GPIO |


## 9. LED 模块

### 9.1 RGB LED（三通道独立控制）

| 子模块 | 信号 | 控制 IO | 电源 | 备注 |
|---|---|---|---|---|
| `RGB LED` | `R（红色通道）` | `EXIO_NUM3` | `3V3_SYS` 或按器件要求供电 | 通过 XL9555 控制，高电平点亮 |
| `RGB LED` | `G（绿色通道）` | `EXIO_NUM4` | 同上 | 通过 XL9555 控制，高电平点亮 |
| `RGB LED` | `B（蓝色通道）` | `EXIO_NUM5` | 同上 | 通过 XL9555 控制，高电平点亮 |
| `RGB LED` | 公共脚 | `3V3_SYS`（共阳）/<br>`GND`（共阴） | — | 限流电阻 R/G/B 各串联 220Ω~1kΩ（按 LED 规格计算） |

### 9.2 说明

| 项目 | 结论 |
|---|---|
| `RGB LED` 方案 | 三通道独立控制，走 `XL9555` EXIO（`EXIO_NUM3/4/5` 分别对应 R/G/B） |
| 为什么不走 ESP32 GPIO | `IO26/IO27` 被 Octal PSRAM 占用不可用；三通道普通 LED 不需要严格时序 |
| 为什么不用 WS2812 | `WS2812/SK6812` 需要严格 800kHz 时序，`XL9555`（I2C 扩展 IO）无法满足，必须直连 ESP32 GPIO 且占用一个可用 IO |
| 颜色能力 | 组合 R/G/B 可混出 8 种颜色（红/绿/蓝/黄/青/紫/白/灭） |
| 亮度调节 | `XL9555` 不支持硬件 PWM；如需调光，可在软件中定时翻转 EXIO 模拟 PWM，或直接固定亮度 |

---

## 10. XL9555 模块

| 子模块 | 信号 | 连接到 | 备注 |
|---|---|---|---|
| `XL9555` | `I2C_SCL` | `IO42` 所在 `I2C0` 总线 | 与 `ES8388` 共总线 |
| `XL9555` | `I2C_SDA` | `IO41` 所在 `I2C0` 总线 | 与 `ES8388` 共总线 |
| `XL9555` | `INT` | `IO19` | 独立外部中断输入；该脚是原生 `USB D-`，前提是不使用原生 USB |
| `XL9555` | `P0/P1 口` | `EXIO_NUM2/8/9/...` | 只做慢速控制 |

### 推荐用途

| EXIO | 用途 |
|---|---|
| `EXIO_NUM2` | `SPK_PA_EN` |
| `EXIO_NUM3` | `RGB_LED_R` |
| `EXIO_NUM4` | `RGB_LED_G` |
| `EXIO_NUM5` | `RGB_LED_B` |
| `EXIO_NUM8` | `LCD_BL_EN` |
| `EXIO_NUM9` | `Touch_RST` |
| 其余空余 `EXIO` | 预留给按键、其它低速使能脚 |

| 说明项 | 结论 |
|---|---|
| `XL9555 INT` 为何能接 `IO19` | 官方 GPIO ISR 服务支持按 GPIO 注册中断处理 |
| `BL_PWM` 为何能接 `IO20` | 官方 `LEDC` 支持把 PWM 路由到指定 GPIO |
| `IO19/20` 为什么还能用 | 官方允许把 USB PHY 引脚配置成普通 GPIO |
| 代价 | 后续不能再把 `IO19/20` 用作原生 USB |

---

## 11. 电源模块

| 电源支路 | 电压 | 建议连续电流能力 | 主要负载 | 备注 |
|---|---:|---:|---|---|
| `VIN` | `5V` | `≥ 3A` | 全板 | 主输入 |
| `5V_LCD` | `5V` | `1.0A` 级 | `4.3寸 RGBLCD`、背光 | 单独加宽 |
| `5V_AMP` | `5V` | `0.5A ~ 1.0A` 级 | 功放、喇叭外放 | 单独加宽，近端储能 |
| `3V3_SYS` | `3.3V` | `0.8A ~ 1.0A` 级 | `ESP32-S3`、`ES8388`、`Touch`、`XL9555`、`RGB LED` | 推荐 Buck 供电 |

---

## 12. 整机功耗预算

| 场景 | 估算总电流 | 估算总功率 | 备注 |
|---|---:|---:|---|
| 轻载 | `0.8A ~ 1.1A @ 5V` | `4W ~ 5.5W` | 屏幕中亮、Wi‑Fi 在线、无大音量外放 |
| 重载 | `1.1A ~ 1.5A @ 5V` | `5.5W ~ 7.5W` | 屏幕高亮、录音+播放、Wi‑Fi 活跃 |

| 项目 | 建议 |
|---|---|
| 建议最小电源 | `5V 2A` |
| 更推荐电源 | `5V 3A` |
| 布线重点 | 先保证 `LCD背光` 和 `功放` 的电源、地回流 |

---

## 13. 最终 IO 分配总表

| 模块 | 信号 | ESP32-S3 IO / EXIO |
|---|---|---|
| `UART0` | `U0TXD` | `IO43` |
| `UART0` | `U0RXD` | `IO44` |
| `BOOT` | `GPIO0` | `IO0` |
| `RGB LCD` | `DE/PCLK/B3~B7/G2~G7/R3~R7` | `IO4/5/17/16/15/7/6/10/9/46/3/8/18/45/48/47/21/14` |
| `RGB LCD` | `BL_PWM` | `IO20` |
| `Touch` | `SCL/SDA/INT/RST` | `IO38/39/40/EXIO_NUM9` |
| `Audio Codec` | `I2C0_SCL/SDA` | `IO42/41` |
| `I2S` | `MCLK/BCLK/LRCK/DOUT/DIN` | `IO11/12/13/1/2` |
| `RGB LED` | `R/G/B` | `EXIO_NUM3/4/5` |
| `XL9555` | `INT` | `IO19` |
| `功放` | `PA_EN` | `EXIO_NUM2` |
| `背光` | `BL_EN` | `EXIO_NUM8` |

---

## 14. 结论

| 项目 | 最终结论 |
|---|---|
| `Type-C` 串口下载/日志 | 通过 `USB-UART` 接 `UART0`，固定占 `IO43/44` |
| `BOOT/自动下载` | `GPIO0` 只给 `BOOT + DTR/RTS 自动下载链路` |
| `I2S` | 用 `IO11/12/13/1/2` |
| `RGB LED` | 三通道普通 RGB LED，通过 `XL9555` 的 `EXIO_NUM3/4/5` 分别控制 R/G/B |
| `XL9555 外部中断` | 用 `IO19`，代价是占掉原生 `USB D-` |
| `LCD 背光 PWM` | 用 `IO20`，代价是占掉原生 `USB D+` |
| `XL9555` | 只做慢速控制，不碰高速/时序信号 |
| 原生 USB | 不使用 |

---

## 15. 禁用引脚清单

以下 GPIO 被 Octal SPI Flash / PSRAM 占用，**不可用于任何外部连接**：

| 禁用 IO | 占用原因 |
|---|---|
| `IO26` | PSRAM CS（`SPICS1`） |
| `IO27` | Octal Flash/PSRAM 数据线（`SPIHD`） |
| `IO28` | Octal Flash/PSRAM 数据线（`SPIWP`） |
| `IO29` | Flash CS（`SPICS0`） |
| `IO30` | Flash/PSRAM CLK（`SPICLK`） |
| `IO31` | Flash/PSRAM 数据线（`SPIQ`） |
| `IO32` | Flash/PSRAM 数据线（`SPID`） |
| `IO33` | Octal Flash/PSRAM 数据线（`SPIIO4`） |
| `IO34` | Octal Flash/PSRAM 数据线（`SPIIO5`） |
| `IO35` | Octal Flash/PSRAM 数据线（`SPIIO6`） |
| `IO36` | Octal Flash/PSRAM 数据线（`SPIIO7`） |
| `IO37` | Octal Flash/PSRAM 数据线（`SPIDQS`） |

> 以上结论基于 `CONFIG_SPIRAM_MODE_OCT=y`（当前 `sdkconfig` 配置）。
> 如更换为不带 PSRAM 或 Quad PSRAM 的模组，可用引脚范围可能扩大，需重新审查。
