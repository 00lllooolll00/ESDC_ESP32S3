# main/app/ —— 应用层（含 app_tts.c 等，CMake configure 时扫描 src 目录）
# main/app/ — 应用层源文件和头文件
# 被 main/CMakeLists.txt include，不独立调用 idf_component_register

set(app_src_dirs
    ${CMAKE_CURRENT_LIST_DIR}/src
)

set(app_include_dirs
    ${CMAKE_CURRENT_LIST_DIR}/inc
)
