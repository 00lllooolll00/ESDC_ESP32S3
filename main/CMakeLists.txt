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
    WHOLE_ARCHIVE
)

# LVGL editor 生成的代码需要 LV_LVGL_H_INCLUDE_SIMPLE 来正确 include lvgl.h
target_compile_definitions(${COMPONENT_LIB} PUBLIC LV_LVGL_H_INCLUDE_SIMPLE)

# 将 ui/ 下的 .bin 文件打包到 vfs FAT 分区
set(vfs_staging "${CMAKE_CURRENT_BINARY_DIR}/vfs_stage")

file(GLOB_RECURSE bin_files CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.bin")

set(copy_cmds)
foreach(bin_file ${bin_files})
    get_filename_component(file_name ${bin_file} NAME)
    list(APPEND copy_cmds COMMAND ${CMAKE_COMMAND} -E copy_if_different "${bin_file}" "${vfs_staging}/${file_name}")
endforeach()
# 将 esp-sr 的中文 TTS voice 数据打包到 vfs FAT 分区
set(esp_sr_tts_dir "${CMAKE_CURRENT_LIST_DIR}/../managed_components/espressif__esp-sr/esp-tts/esp_tts_chinese")
list(APPEND copy_cmds COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${esp_sr_tts_dir}/esp_tts_voice_data_xiaole.dat"
    "${vfs_staging}/esp_tts_voice_data_xiaole.dat")

add_custom_target(vfs_staging
    COMMAND ${CMAKE_COMMAND} -E make_directory ${vfs_staging}
    ${copy_cmds}
    COMMENT "Auto-syncing .bin VFS assets..."
)

fatfs_create_rawflash_image(vfs ${vfs_staging} FLASH_IN_PROJECT)
add_dependencies(fatfs_vfs_bin vfs_staging)
