# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概况

ESP32-S3 嵌入式项目，基于 **ESP-IDF v5.5.3**，使用 **LVGL v9.5.0** 作为 GUI 框架。目标芯片为 ESP32S3，16MiB Flash 分区。

## 常用命令

项目使用 [just](https://github.com/casey/just) 作为任务运行器：

| 命令 | 说明 |
|------|------|
| `just build` / `just b` | 构建项目（含 size 输出） |
| `just clean` / `just c` | 完全清理（fullclean） |
| `just flash-by-usb` / `just f` | 构建并通过 `/dev/ttyACM0` 烧录 + 监视 |
| `just flash-by-serial` | 构建并通过 `/dev/ttyUSB0` 烧录 + 监视 |
| `just run-by-usb` / `just r` | 构建 + USB 烧录 + 监视 |
| `just run-by-serial` | 构建 + 串口烧录 + 监视 |
| `just monitor` / `just m` | 仅打开串口监视器 |
| `just menuconfig` / `just cfg` | 打开 menuconfig 配置界面 |

底层使用 `idf.py`，也可直接调用：`idf.py build`、`idf.py flash -p /dev/ttyACM0`、`idf.py monitor` 等。

## 架构分层

```
main (入口: app_main → app_init)
 ├── app            应用层：任务创建、GUI 初始化、业务逻辑
 ├── bsp            板级支持包：LED、按键、IO 扩展、I2C、LCD、WiFi
 ├── common         公共头：user_common.h（FreeRTOS + ESP_LOG + ek_def 统一入口）
 └── middlewares     中间件：ek_utils 工具库、LVGL 显示端口适配
```

### 组件依赖关系

- `app` → `bsp`, `common`, `middlewares`
- `middlewares` → `common`, `bsp`, `lvgl`（通过 ESP-IDF Component Manager 管理）
- `bsp` → `driver`, `esp_lcd`, `esp_psram`, `esp_event`, `esp_wifi`, `common`, `middlewares`
- `common` → 无依赖（纯头文件组件）

### 关键设计约定

- **统一头文件入口**：所有 `.c` 文件通过 `#include "user_common.h"` 引入 FreeRTOS、ESP_LOG、ek_def 等基础设施，不再单独包含
- **日志宏**：使用 `FILE_TAG("模块名")` 定义模块标签，配合 `LOG_INFO`/`LOG_ERROR` 等宏输出带标签日志（底层封装 ESP_LOG）
- **BSP 层**：所有硬件抽象放在 `components/bsp/` 下，每个外设一个子目录（含 `.c`/`.h`），通过 `bsp/CMakeLists.txt` 的 `src_dirs` 和 `include_dirs` 注册
- **LVGL 显示端口**：`middlewares/lvgl_v9_5/` 提供 `lv_port_disp_init()`，在 `app_init()` 中调用
- **LVGL 线程安全**：在 GUI 任务之外操作 LVGL 对象时，使用 `lv_lock()`/`lv_unlock()` 保护（参考 `app_key_task`）

### Flash 分区 (16MiB)

| 分区名 | 类型 | 偏移 | 大小 |
|--------|------|------|------|
| nvs | data/nvs | 0x9000 | 0x6000 |
| phy_init | data/phy | 0xf000 | 0x1000 |
| factory | app/factory | 0x10000 | 0x1F0000 (~2MB) |
| vfs | data/fat | 0x200000 | 0xA00000 (10MB) |
| storage | data/spiffs | 0xC00000 | 0x400000 (4MB) |

### ek_utils 工具库

位于 `components/middlewares/ek_utils/`，提供跨编译器兼容层（`ek_def.h`）、动态内存（TLSF）、日志、环形缓冲、Shell、向量等工具。第三方库以源码形式集成在 `third_party/` 下（TLSF 内存分配器、lwprintf）。

### 编译选项

根 `CMakeLists.txt` 设置了 `-fdiagnostics-color=always`、`-ffast-math`、`-Wno-error=format`、`-Wno-format`。启用 `CMAKE_EXPORT_COMPILE_COMMANDS` 供 clangd 使用。

### 开发环境

- **Dev Container**：支持 ESP-IDF QEMU 环境（`privileged` 模式）
- **clangd**：配置了 ESP-specific clangd 路径，compile_commands 指向 build 目录
- **依赖管理**：通过 ESP-IDF Component Manager 管理 `lvgl/lvgl`（版本 ^9.2.0，实际锁定 9.5.0）
