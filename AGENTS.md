# Repository Guidelines

## Project Overview

- 这是一个基于 `ESP-IDF 5.5.4` 的 `ESP32-S3` 固件项目，目标是带 `800×480` 触摸屏的嵌入式显示终端。
- 主要能力集中在 `LVGL` 图形界面、`WiFi`、`LED`、按键，以及已分层接入的 LCD / Touch / SDCard / Audio / MQTT 等外设能力。
- UI 使用 `LVGL Editor` 通过编写 `XML` 文件生成（工作目录 `main/ui/lvgl_editor/`），运行时依赖 `LVGL 9.5.0`；二进制 UI 资源会被打包进 `vfs` FAT 分区。

## Architecture & Data Flow

- 分层固定为：`components/bsp` → `components/platform` → `components/impl` → `main/app` → `main/ui`。
- `main/main.c` 是启动总线：`NVS` 初始化（必要时擦除重建）→ 挂载只读 `/vfs` → 注册 `impl_*` 设备 → 初始化 `LVGL` 显示/触摸端口 → 启动 `app_wifi_init()` / `app_led_init()` / `app_key_intit()` / `app_ui_init()`。
- `components/platform/inc/plat_base.h` 定义统一设备抽象：`plat_dev_t + plat_dev_ops_t + state + lock`。上层不要绕过 `plat_*` 直接碰底层驱动。
- `components/impl/src/impl_*.c` 负责把 `ESP-IDF` / `BSP` 适配到 `plat_*` 接口；例如 `components/impl/src/impl_wifi.c` 把 WiFi 扫描、连接、状态回调桥接为 `plat_wifi_*`。
- `main/app/src/app_*.c` 是任务级业务编排层：通常一个功能一个 `FreeRTOS` task。例：`main/app/src/app_wifi.c` 用队列接收 `SCAN/CONNECT/DISCONNECT` 命令，再把结果通过回调送回 UI。
- `main/ui` 是界面层：`lvgl_editor/` 下以 `XML` 为事实来源，由 LVGL Editor 生成 `*_gen.c` / `*_gen.h`；`lvgl_editor/ui.c` / `ui.h` 与 `actions/*.c` 是手写层（回调实现等）。
- 典型 WiFi 数据流：XML `event_cb` 绑定的 `main/ui/actions/wifi_actions.c` 回调 → `app_wifi_send_cmd()` → `main/app/src/app_wifi.c` 队列任务 → `plat_wifi_*` / `impl_wifi.c` → 回调返回 UI，并在 `lv_lock()` / `lv_unlock()` 保护下更新界面。
- UI 资源流：`main/CMakeLists.txt` 会把 `main/ui/*.bin` 打进 `vfs` 镜像；`main/app/src/app_ui.c` 运行时加载 `S:/ui_font_chinese_3500_14.bin`，然后驱动 `ui_init()` / `ui_tick()`。

## Key Directories

- `main/`：入口与顶层装配。
- `main/app/`：应用层任务与跨层编排，`inc/` 头文件、`src/` 实现。
- `main/ui/`：LVGL UI（LVGL Editor + XML 工作流）。重点区分：
  - XML 事实来源（可编辑）：`main/ui/lvgl_editor/project.xml`（项目配置）、`globals.xml`（样式 / subject / 常量）、`screens/*.xml`（屏幕，以 `<screen>` 标签开头）、`components/*.xml`（可复用组件，以 `<component>` 标签开头）、`widgets/`（自定义 widget）
  - 生成产物（禁止手改，文件头标注 "Generated file, do not edit"）：`main/ui/lvgl_editor/ui_gen.c` / `ui_gen.h`、`screens/*_gen.*`、`components/*_gen.*`、`file_list_gen.cmake` / `component_lib_list_gen.cmake`
  - 手写（可编辑，不会被生成器覆盖）：`main/ui/lvgl_editor/ui.c` / `ui.h`（入口、导航与面板回调）、`main/ui/actions/*.c`（业务行为回调）、`user_config.cmake`（追加自定义源文件）
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

- C 文件通常先引入 `common_header.h`，再用 `EK_LOG_FILE_TAG("...")` 声明日志标签；统一使用 `EK_LOG_DEBUG/INFO/WARN/ERROR`。
- 错误处理遵循就近模式：`ESP_ERROR_CHECK`、`assert()`、以及少量 `0 / -1` 返回约定并存；扩展时优先复用相邻文件风格，不要自造第四种约定。
- 命名有明显层前缀：`app_*`、`plat_*`、`impl_*`、`bsp_*`。变量常见前缀：全局 `g_`、静态 `s_`、内部 helper `_name`。
- **注释规范**：
  - 语句注释统一使用 `// ...`，不要使用 `/* ... */`。
  - 函数声明注释使用 doxygen 风格（`/** ... */`），用于描述函数用途、参数、返回值等接口契约。
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
- UI 数据走 LVGL subject 机制：在 `globals.xml` 的 `<subjects>` 中声明（如 `wifi_is_connected`、`volume`），生成 `lv_subject_t` 全局变量（见 `ui_gen.h`），用 `lv_subject_*` API 读写。
- 生成的 UI 控件 / 屏幕通过 `ui_gen.h` 及各 `*_gen.h` 中 extern 的全局 `lv_obj_t *`（如 `main_page`、`weather`）访问；改动 XML 中的 name 会直接影响 `ui.c` 与 `actions/*.c` 里的引用。
- **重要边界（不可违反）**：修改 LVGL 界面（布局、控件树、z-order 层级、样式、subject 等）时，只能改 `main/ui/lvgl_editor/` 下的 `XML` 文件（`project.xml` / `globals.xml` / `screens/*.xml` / `components/*.xml`），再用 LVGL Editor 重新生成。绝对不要直接修改生成产物，包括：`ui_gen.c` / `ui_gen.h`、`screens/*_gen.*`、`components/*_gen.*`、`file_list_gen.cmake` / `component_lib_list_gen.cmake` 等（文件头均标注 "Generated file, do not edit"）。直接改生成代码会被下次重新生成覆盖，且破坏工程一致性。
- **回调函数例外**：XML 中通过 `event_cb="函数名"` 引用的回调，其函数体在 `main/ui/lvgl_editor/ui.c`（面板显示/隐藏、`nav_back` 等）或 `main/ui/actions/*.c`（业务 action，如 `action_wifi_start_scan`）中手写实现，这部分可编辑且不会被生成器覆盖；但回调的原型声明由 `ui_gen.h` 生成，不要手改声明，只改实现。

## Important Files

- `main/main.c`：启动顺序与设备装配的唯一入口。
- `main/app/src/app_ui.c`：UI 任务、字体加载、`ui_tick()` 驱动。
- `main/app/src/app_wifi.c`：WiFi 队列任务、状态回调到 UI 的桥梁。
- `main/ui/lvgl_editor/` 下的 `XML`：UI 设计事实来源；改布局先改这里（屏幕改 `screens/*.xml`，组件改 `components/*.xml`，样式/数据改 `globals.xml`）。
- `main/ui/actions/wifi_actions.c`：WiFi UI 行为、密码输入、连接状态提示。
- `main/ui/actions/volume_actions.c`：音量滑块与图标联动。
- `main/ui/lvgl_editor/globals.xml`：UI 全局样式、subject（如 `wifi_is_connected` / `volume`）、常量的事实来源。
- `main/ui/lvgl_editor/ui.c` / `ui.h`：手写 UI 入口（`ui_init()`、字体/图标设置）与 `event_cb` 回调实现（`nav_back`、面板显示/隐藏）。
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
- `just format` 可用于普通 C 代码，但不会处理 `main/ui/*`，因为其中含大量 LVGL Editor 生成产物（`*_gen.*`）与手写 UI 代码混排。
- 仓库未发现 CI、覆盖率工具或统一 lint 入口；不要在说明里假设存在自动化 QA 管线。
