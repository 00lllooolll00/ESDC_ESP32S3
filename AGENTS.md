# Repository Guidelines

## Project Overview

- 这是一个基于 `ESP-IDF 5.5.4` 的 `ESP32-S3` 固件项目，目标是带 `800×480` 触摸屏的嵌入式显示终端。
- 主要能力集中在 `LVGL` 图形界面、`WiFi`、`LED`、按键，以及已分层接入的 LCD / Touch / SDCard / Audio / MQTT 等外设能力。
- UI 使用 `EEZ Studio + EEZ Flow` 生成，运行时依赖 `LVGL 9.5.0`；二进制 UI 资源会被打包进 `vfs` FAT 分区。

## Architecture & Data Flow

- 分层固定为：`components/bsp` → `components/platform` → `components/impl` → `main/app` → `main/ui`。
- `main/main.c` 是启动总线：`NVS` 初始化（必要时擦除重建）→ 挂载只读 `/vfs` → 注册 `impl_*` 设备 → 初始化 `LVGL` 显示/触摸端口 → 启动 `app_wifi_init()` / `app_led_init()` / `app_key_intit()` / `app_ui_init()`。
- `components/platform/inc/plat_base.h` 定义统一设备抽象：`plat_dev_t + plat_dev_ops_t + state + lock`。上层不要绕过 `plat_*` 直接碰底层驱动。
- `components/impl/src/impl_*.c` 负责把 `ESP-IDF` / `BSP` 适配到 `plat_*` 接口；例如 `components/impl/src/impl_wifi.c` 把 WiFi 扫描、连接、状态回调桥接为 `plat_wifi_*`。
- `main/app/src/app_*.c` 是任务级业务编排层：通常一个功能一个 `FreeRTOS` task。例：`main/app/src/app_wifi.c` 用队列接收 `SCAN/CONNECT/DISCONNECT` 命令，再把结果通过回调送回 UI。
- `main/ui` 是界面层：`ui.c` / `screens.c` / `styles.c` / `screens.h` / `eez-flow.*` 为 EEZ 生成；`actions/*.c` 与 `vars/*.c` 是手写桥接层。
- 典型 WiFi 数据流：`main/ui/actions/wifi_actions.c` 里的 LVGL 事件 → `app_wifi_send_cmd()` → `main/app/src/app_wifi.c` 队列任务 → `plat_wifi_*` / `impl_wifi.c` → 回调返回 UI，并在 `lv_lock()` / `lv_unlock()` 保护下更新界面。
- UI 资源流：`main/CMakeLists.txt` 会把 `main/ui/*.bin` 打进 `vfs` 镜像；`main/app/src/app_ui.c` 运行时加载 `S:/ui_font_chinese_3500_14.bin`，然后驱动 `ui_init()` / `ui_tick()`。

## Key Directories

- `main/`：入口与顶层装配。
- `main/app/`：应用层任务与跨层编排，`inc/` 头文件、`src/` 实现。
- `main/ui/`：LVGL / EEZ UI。重点区分：
  - 自动生成：`main/ui/ui.c`、`main/ui/screens.c`、`main/ui/styles.c`、`main/ui/screens.h`、`main/ui/eez-flow.*`
  - 手写：`main/ui/actions/*.c`、`main/ui/vars/*.c`
  - UI 源文件：`main/ui/eez/eez.eez-project`
- `components/common/inc/`：公共宏、日志、错误字符串等基础设施。
- `components/platform/`：平台抽象层，定义 `plat_*` 设备接口与共性状态机。
- `components/impl/`：具体实现层，把 `platform` 接口接到 `BSP` / `ESP-IDF`。
- `components/bsp/`：板级/芯片级底层驱动。
- `components/middlewares/`：内置中间件，当前主要是 `ek_utils` 与 `lvgl_v9_5`。
- `managed_components/`：生成目录，已被 `.gitignore` 忽略；不要把它当成手工维护源码。

## Development Commands

- `just build`：构建并输出 size 信息（实际调用 `idf.py build size`）。
- `just flash`：仅烧录 app 分区（`idf.py app-flash -b 2000000`，2Mbps 高速；依赖 `build`）。
- `just flash-full`：整片烧录（`idf.py flash -b 2000000`，2Mbps 高速；依赖 `build`）。
- `just monitor`：打开串口监视。
- `just run`：`build + flash + monitor` 一条龙（先编译再烧录再监视）。
- `just menuconfig`：打开 `ESP-IDF` 配置界面。
- `just clean`：删除 `build/`。
- `just format`：对 `components/**/*.c,h` 与 `main/**/*.c,h` 运行 `clang-format`，但明确排除 `main/ui/*`。
- 仓库里没有 `just test`，也没有项目级 `pytest` / `ctest` 入口。
- 命令别名（`justfile` 顶部 `alias`，日常高频）：`just b`=build、`just c`=clean、`just f`=flash、`just ff`=flash-full、`just r`=run、`just cfg`=menuconfig、`just m`=monitor、`just fmt`=format。

## Code Conventions & Common Patterns

- C 文件通常先引入 `common_header.h`，再用 `FILE_TAG("...")` 声明日志标签；统一使用 `LOG_DEBUG/INFO/WARN/ERROR`。
- 错误处理遵循就近模式：`ESP_ERROR_CHECK`、`assert()`、以及少量 `0 / -1` 返回约定并存；扩展时优先复用相邻文件风格，不要自造第四种约定。
- 命名有明显层前缀：`app_*`、`plat_*`、`impl_*`、`bsp_*`。变量常见前缀：全局 `g_`、静态 `s_`、内部 helper `_name`。
- **注释规范**：
  - 函数/语句注释统一使用 `// ...`，不要使用 `/* ... */`。
  - 注释写在代码上方，不要写在行尾。正确写法是多行注释逐行 `//`，最后跟代码：
    ```
    // 第一句注释
    // 第二句注释
    // 第三句注释
    // ...
    func();
    ```
    而不是 `func(); // ...`。
  - 只在确有必要说明的一段加注释，不要给每一句代码都加注释。
- 平台设备要走 `plat_dev_register()` / `plat_*_dev_init()` 生命周期，并尊重 `PLAT_DEV_LOCK` / `PLAT_DEV_UNLOCK` 状态与锁语义。
- `main/app/src/app_*.c` 偏向“一功能一任务”；跨线程触碰 LVGL 时必须像 `main/app/src/app_wifi.c` 那样包 `lv_lock()` / `lv_unlock()`。
- UI 原生变量遵循 `get_var_g_*` / `set_var_g_*` 命名，见 `main/ui/vars/wifi_vars.c`、`main/ui/vars/volume_vars.c`。
- 自动生成的 UI 控件通过全局 `objects` 访问，定义在 `main/ui/screens.h`。改动对象名会直接影响 `actions/*.c`。
- **重要边界**：如果改的是 UI 布局、控件树、Flow 绑定，先改 `main/ui/eez/eez.eez-project`，再重新生成代码；不要把持久修改直接写进 `main/ui/ui.c` 或 `main/ui/screens.c`。
- `main/ui/eez-flow.cpp` 和 `main/ui/eez-flow.h` 体积很大且为框架/生成产物；非必要不要做大范围人工修改。

## Important Files

- `main/main.c`：启动顺序与设备装配的唯一入口。
- `main/app/src/app_ui.c`：UI 任务、字体加载、`ui_tick()` 驱动。
- `main/app/src/app_wifi.c`：WiFi 队列任务、状态回调到 UI 的桥梁。
- `main/ui/eez/eez.eez-project`：UI 设计事实来源；改布局先改这里。
- `main/ui/actions/wifi_actions.c`：WiFi UI 行为、密码输入、连接状态提示。
- `main/ui/actions/volume_actions.c`：音量滑块与图标联动。
- `main/ui/vars/wifi_vars.c`、`main/ui/vars/volume_vars.c`：EEZ native vars 手写桥接。
- `main/CMakeLists.txt`：主组件注册，以及 `main/ui/*.bin` → `vfs` 镜像的打包逻辑。
- `justfile`：本仓库最直接的开发命令入口。
- `sdkconfig`：项目级 `ESP-IDF` 配置，包含目标、Flash、PSRAM、断言等行为。
- `partitions-16MiB.csv`：分区布局，定义 `vfs` FAT 分区。
- `dependencies.lock`：锁定 `ESP-IDF 5.5.4`、`LVGL 9.5.0` 和目标 `esp32s3`。
- `plan.md`：当前唯一业务文档，含 UI 变更方案与手工验收清单。

## Runtime/Tooling Preferences

- 目标运行时是 `ESP32-S3 + ESP-IDF`；不是 `Node` / `Bun` / `Python` 应用仓库。
- 构建系统固定是 `CMake + idf.py`，本地操作优先走 `justfile` 封装命令。
- 外部依赖通过 `main/idf_component.yml` + `dependencies.lock` 管理；当前唯一显式第三方依赖是 `lvgl/lvgl`。
- `CMakeLists.txt` 已开启 `CMAKE_EXPORT_COMPILE_COMMANDS ON`；编辑器/LSP 预期可用 `compile_commands.json`。
- `.vscode/settings.json` 里绑定了本地 `ESP-IDF`、`esp-clang`、`OpenOCD` 与串口路径；如果工具链问题只影响编辑器，不代表仓库结构异常。
- `main/ui/*.bin` 会被打进 `vfs` 分区；新增 UI 二进制资源时，优先复用这一打包路径。

## Testing & QA

- 项目没有正式测试框架：无 `test/`、无 `tests/`、无项目级 `pytest` / `ctest` / `ESP-IDF unit test app`。
- 默认验证方式是：
  - `just build`：确认编译通过。
  - `just run`：烧录到板子并观察串口输出。
  - UI/交互改动：按 `plan.md` 的清单做手工验收。
- 代码里的主要质量护栏是 `assert()`、`ESP_ERROR_CHECK()`、设备状态机、以及启动期日志（NVS / VFS / 外设初始化）。
- `just format` 可用于普通 C 代码，但不会处理 `main/ui/*`，因为那里大多是自动生成文件。
- 仓库未发现 CI、覆盖率工具或统一 lint 入口；不要在说明里假设存在自动化 QA 管线。
