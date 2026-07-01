cmake_minimum_required(VERSION 3.16)

set(CMAKE_PROJECT_NAME esp32s3_main)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(${CMAKE_PROJECT_NAME})

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
