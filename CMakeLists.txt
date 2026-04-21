cmake_minimum_required(VERSION 3.16)

set(CMAKE_PROJECT_NAME esp32s3_main)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

idf_build_set_property(COMPILE_OPTIONS
    "-fdiagnostics-color=always"
    "-ffast-math"
    "-Wno-error=format"
    "-Wno-format"

    APPEND
)


project(${CMAKE_PROJECT_NAME})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

