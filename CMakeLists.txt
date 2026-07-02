cmake_minimum_required(VERSION 3.16)

set(CMAKE_PROJECT_NAME esp32s3_main)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(${CMAKE_PROJECT_NAME})

# 每次 CMake 配置时自动重新生成 LVGL editor C 代码
execute_process(
    COMMAND node ${CMAKE_CURRENT_SOURCE_DIR}/script/lvgl_editor/lved-cli.js generate
        ${CMAKE_CURRENT_SOURCE_DIR}/main/ui/lvgl_editor
        --toolchain-mode none
        --ignore-fonts
        --ignore-images
        --skip-resources
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE _lved_result
)
if(_lved_result AND NOT _lved_result EQUAL 0)
    message(FATAL_ERROR "LVGL editor code generation failed (exit code: ${_lved_result}). Run 'node script/lved-cli.js generate main/ui/lvgl_editor --toolchain-mode none --ignore-fonts --ignore-images --skip-resources' manually to see errors.")
endif()

idf_build_set_property(COMPILE_OPTIONS
    "-fdiagnostics-color=always"
    "-ffast-math"
    "-Wno-error=format"
    "-Wno-format"
    APPEND
)

foreach(_comp main middlewares bsp impl platform)
    if(TARGET __idf_${_comp})
        target_compile_options(__idf_${_comp} PRIVATE -Wno-error=enum-compare)
    endif()
endforeach()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
