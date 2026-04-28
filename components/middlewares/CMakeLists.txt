# 源文件
set(src_dirs
            ek_utils/src
            ek_utils/third_party/tlsf
            ek_utils/third_party/lwprintf
            lvgl_v9_5/src
)

# 头文件
set(include_dirs
            ek_utils/inc
            ek_utils/third_party/tlsf
            ek_utils/third_party/lwprintf/inc
            lvgl_v9_5/inc
)

# 依赖项
set(requires
            platform
            lvgl
)

# 注册到 idf
idf_component_register(
            SRC_DIRS ${src_dirs}
            INCLUDE_DIRS ${include_dirs}
            REQUIRES ${requires}
)

# 当前组件的编译选项
component_compile_options(
)

