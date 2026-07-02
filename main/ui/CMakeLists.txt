# main/ui/ — UI 层源文件和头文件（LVGL editor 生成）
# 被 main/CMakeLists.txt include，不独立调用 idf_component_register

set(ui_src_dirs
    ${CMAKE_CURRENT_LIST_DIR}/actions
    ${CMAKE_CURRENT_LIST_DIR}/lvgl_editor
    ${CMAKE_CURRENT_LIST_DIR}/lvgl_editor/components
    ${CMAKE_CURRENT_LIST_DIR}/lvgl_editor/screens
)

set(ui_include_dirs
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lvgl_editor
)
