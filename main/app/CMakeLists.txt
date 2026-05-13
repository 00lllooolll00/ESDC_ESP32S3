# main/app/ — 应用层源文件和头文件
# 被 main/CMakeLists.txt include，不独立调用 idf_component_register

set(app_src_dirs
    ${CMAKE_CURRENT_LIST_DIR}/src
)

set(app_include_dirs
    ${CMAKE_CURRENT_LIST_DIR}/inc
)
