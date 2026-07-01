# ESP32-S3 新板 IO 引脚分布图

> 基于 `ESP32S3_RGB_Touch_I2S建议引脚图.md` (2026-06-19)。
> 含 Strapping 引脚约束、禁用引脚清单。

---

## 一、启动 & 串口下载

| IO | 信号 | 连接 | 备注 |
|---|---|---|---|
| `IO0` | `BOOT` | BOOT 按键 + USB-UART DTR | 仅用于 BOOT / 自动下载 |
| `EN` | `RESET` | RESET 按键 + USB-UART RTS | 芯片复位，不可复用 |
| `IO43` | `U0TXD` | UART0 TX → USB-UART RXD | ESP32-S3 UART0 默认脚 |
| `IO44` | `U0RXD` | UART0 RX → USB-UART TXD | ESP32-S3 UART0 默认脚 |

---

## 二、RGB LCD（16bit DE 模式，565 格式）

| LCD 信号 | IO | 备注 |
|---|---|---|
| `DE` | `IO4` | 数据使能 |
| `PCLK` | `IO5` | 像素时钟 |
| `R3` | `IO45` | ⚠️ Strapping：VDD_SPI 电压选择，复位必须高电平 |
| `R4` | `IO48` | |
| `R5` | `IO47` | |
| `R6` | `IO21` | |
| `R7` | `IO14` | |
| `G2` | `IO10` | |
| `G3` | `IO9` | |
| `G4` | `IO46` | ⚠️ Strapping：启动模式，复位必须高电平 |
| `G5` | `IO3` | ⚠️ Strapping：JTAG 信号源选择，复位必须高电平 |
| `G6` | `IO8` | |
| `G7` | `IO18` | |
| `B3` | `IO17` | |
| `B4` | `IO16` | |
| `B5` | `IO15` | |
| `B6` | `IO7` | |
| `B7` | `IO6` | |
| `BL_PWM` | `IO20` | 背光亮度 PWM（LEDC）；前提：不使用原生 USB（占 USB D+） |
| `BL_EN` | `EXIO8` | 背光总使能，高有效 |

---

## 三、触摸模块（I2C1 独立总线）

| IO / EXIO | 信号 | 备注 |
|---|---|---|
| `IO38` | `CT_SDA` | 触摸 I2C1 数据 |
| `IO39` | `CT_SCL` | 触摸 I2C1 时钟 |
| `IO40` | `CT_INT` | 触摸中断输入 |
| `EXIO4` | `CT_RST` | 触摸复位，低有效 |

---

## 四、音频 Codec（ES8388）— I2C0 控制总线

| IO | 信号 | 连接 | 备注 |
|---|---|---|---|
| `IO41` | `I2C0_SDA` | ES8388 + XL9555 | 共用 I2C 总线 |
| `IO42` | `I2C0_SCL` | ES8388 + XL9555 | 共用 I2C 总线 |

> `PA_EN` 当前未分配到 XL9555；原 `EXIO2` 已改为 `RGB_B`，如需功放使能需另行分配控制脚。

---

## 五、I2S 音频（全双工：播放 + 录音）

| IO | 信号 | 方向 | 备注 |
|---|---|---|---|
| `IO11` | `MCLK` | ESP32 → ES8388 | 音频主时钟 |
| `IO12` | `BCLK` | ESP32 → ES8388 | 位时钟 |
| `IO13` | `LRCK` | ESP32 → ES8388 | 声道时钟 |
| `IO1` | `DOUT` | ESP32 → ES8388 | 播放数据 |
| `IO2` | `DIN` | ES8388 → ESP32 | 录音数据 |

---

## 六、XL9555 IO 扩展器

> I2C 地址 `0x20`（A0/A1/A2 全接 GND），与 ES8388 共享 I2C0 总线。

### 6.1 XL9555 控制信号

| IO | 信号 | 备注 |
|---|---|---|
| `IO19` | `XL_INT` | XL9555 中断输入；前提：不使用原生 USB（占 USB D-） |
| `IO41` | `I2C0_SDA` | 与 ES8388 共用 |
| `IO42` | `I2C0_SCL` | 与 ES8388 共用 |

### 6.2 XL9555 P0 口（低 8 位）

| EXIO | 用途 | 信号名 | 备注 |
|---|---|---|---|
| `EXIO0` | RGB LED 红 | `RGB_R` | 高电平点亮 |
| `EXIO1` | RGB LED 绿 | `RGB_G` | 高电平点亮 |
| `EXIO2` | RGB LED 蓝 | `RGB_B` | 高电平点亮；原 `PA_EN` 位置已占用 |
| `EXIO3` | 预留 | — | 未分配 |
| `EXIO4` | 触摸复位 | `TOUCH_RST` | 低有效 |
| `EXIO5` | 按键 1 | `KEY_1` | 慢速输入，需上拉 |
| `EXIO6` | 按键 2 | `KEY_2` | 慢速输入，需上拉 |
| `EXIO7` | 按键 3 | `KEY_3` | 慢速输入，需上拉 |

### 6.3 XL9555 P1 口（高 8 位）

| EXIO | 用途 | 信号名 | 备注 |
|---|---|---|---|
| `EXIO8` | 背光总使能 | `LCD_BL_EN` | 高有效 |
| `EXIO9` | 预留 | — | 引出到排针 H1 |
| `EXIO10` | 预留 | — | 引出到排针 H1 |
| `EXIO11` | 预留 | — | 引出到排针 H1 |
| `EXIO12` | 预留 | — | 引出到排针 H1 |
| `EXIO13` | 预留 | — | 引出到排针 H1 |
| `EXIO14` | 预留 | — | 引出到排针 H1 |
| `EXIO15` | 预留 | — | 引出到排针 H1 |

---

## 七、Strapping 引脚约束

> 芯片复位期间采样，**外设必须保证正确电平**，否则启动失败或 Flash 不可用。

| IO | 功能 | 复位时要求 | 外设负载 | 硬件处理 |
|---|---|---|---|---|
| `IO3` | JTAG 信号源选择 | **高电平** | LCD G5 | 外部 10kΩ 上拉至 3.3V |
| `IO45` | VDD_SPI 电压选择 | **高电平**（低→1.8V Flash 不工作） | LCD R3 | 外部 10kΩ 上拉至 3.3V |
| `IO46` | 启动模式 | **高电平** | LCD G4 | 外部 10kΩ 上拉至 3.3V |

> 这三个脚被 LCD 数据线占用。LCD 模组未初始化时数据脚通常为高阻态，上拉电阻保证复位期间电平正确。

---

## 八、禁用引脚清单（Octal PSRAM 占用）

> 基于 `CONFIG_SPIRAM_MODE_OCT=y`（当前 sdkconfig 配置）。
> **这些 IO 不可用于任何外部连接。**

| 禁用 IO | 占用原因 |
|---|---|
| `IO26` | PSRAM CS |
| `IO27` | Flash/PSRAM SPIHD |
| `IO28` | Flash/PSRAM SPIWP |
| `IO29` | Flash CS |
| `IO30` | Flash/PSRAM CLK |
| `IO31` | Flash/PSRAM SPIQ |
| `IO32` | Flash/PSRAM SPID |
| `IO33` | Octal SPIIO4 |
| `IO34` | Octal SPIIO5 |
| `IO35` | Octal SPIIO6 |
| `IO36` | Octal SPIIO7 |
| `IO37` | Octal SPIDQS |

---

## 九、ESP32-S3 GPIO 状态速查

| IO | 状态 | 用途 |
|---|---|---|
| `IO0` | 已用 | BOOT |
| `IO1` | 已用 | I2S DOUT |
| `IO2` | 已用 | I2S DIN |
| `IO3` | 已用 | LCD G5 ⚠️ |
| `IO4` | 已用 | LCD DE |
| `IO5` | 已用 | LCD PCLK |
| `IO6` | 已用 | LCD B7 |
| `IO7` | 已用 | LCD B6 |
| `IO8` | 已用 | LCD G6 |
| `IO9` | 已用 | LCD G3 |
| `IO10` | 已用 | LCD G2 |
| `IO11` | 已用 | I2S MCK |
| `IO12` | 已用 | I2S BCK |
| `IO13` | 已用 | I2S LRCK |
| `IO14` | 已用 | LCD R7 |
| `IO15` | 已用 | LCD B5 |
| `IO16` | 已用 | LCD B4 |
| `IO17` | 已用 | LCD B3 |
| `IO18` | 已用 | LCD G7 |
| `IO19` | 已用 | XL9555 INT（占 USB D-） |
| `IO20` | 已用 | LCD BL_PWM（占 USB D+） |
| `IO21` | 已用 | LCD R6 |
| `IO26` ~ `IO37` | **禁用** | Octal PSRAM |
| `IO38` | 已用 | Touch I2C1 SDA |
| `IO39` | 已用 | Touch I2C1 SCL |
| `IO40` | 已用 | Touch INT |
| `IO41` | 已用 | I2C0 SDA |
| `IO42` | 已用 | I2C0 SCL |
| `IO43` | 已用 | UART0 TX |
| `IO44` | 已用 | UART0 RX |
| `IO45` | 已用 | LCD R3 ⚠️ |
| `IO46` | 已用 | LCD G4 ⚠️ |
| `IO47` | 已用 | LCD R5 |
| `IO48` | 已用 | LCD R4 |
| `EN` | 已用 | 复位 |

> ⚠️ = Strapping 引脚，见第七章约束。

---

## 十、外设总线拓扑

```
ESP32-S3
├── I2C0 (IO41/42) ─── XL9555 (0x20) ─┬── EXIO0 → RGB_R
│                        │              ├── EXIO1 → RGB_G
│                        │              ├── EXIO2 → RGB_B
│                        │              ├── EXIO4 → TOUCH_RST
│                        │              ├── EXIO5 → KEY_1
│                        │              ├── EXIO6 → KEY_2
│                        │              ├── EXIO7 → KEY_3
│                        │              ├── EXIO8 → LCD_BL_EN
│                        │              └── EXIO9~15 → H1 排针预留
│                        │
│                        └── ES8388 (0x10) ─── 模拟输出 → 功放（PA_EN 未分配）→ 喇叭
│
├── I2C1 (IO38=SDA, IO39=SCL) ─── 触摸 IC
│
├── I2S (IO1/2/11/12/13) ─── ES8388
│
├── RGB LCD (IO3~10,14~18,21,45~48) + BL_PWM (IO20)
│
├── UART0 (IO43/44) ─── USB-UART (CH340C) ─── Type-C
│
├── XL9555 INT ←── IO19
│
├── BOOT (IO0)
└── EN
```
