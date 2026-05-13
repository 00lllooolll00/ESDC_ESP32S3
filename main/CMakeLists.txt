# 引入子目录的源文件/头文件声明
include(${CMAKE_CURRENT_LIST_DIR}/app/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/ui/CMakeLists.txt)

# root 级别（main/ 自身）
set(root_src_dirs .)
set(root_include_dirs .)

# 合并后统一注册
set(all_src_dirs
    ${root_src_dirs}
    ${app_src_dirs}
    ${ui_src_dirs}
)

set(all_include_dirs
    ${root_include_dirs}
    ${app_include_dirs}
    ${ui_include_dirs}
)

idf_component_register(
    SRC_DIRS ${all_src_dirs}
    INCLUDE_DIRS ${all_include_dirs}
)

# 将 ui/ 下的 .bin 文件打包到 vfs FAT 分区
set(vfs_staging "${CMAKE_CURRENT_BINARY_DIR}/vfs_stage")

add_custom_target(vfs_staging
    COMMAND ${CMAKE_COMMAND} -E make_directory ${vfs_staging}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/ui/ui_image_main_page_bg.bin"
        "${vfs_staging}/ui_image_main_page_bg.bin"
    COMMENT "Staging VFS assets..."
)

fatfs_create_rawflash_image(vfs ${vfs_staging} FLASH_IN_PROJECT)
add_dependencies(fatfs_vfs_bin vfs_staging)
