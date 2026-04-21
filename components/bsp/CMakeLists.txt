# 源文件
set(src_dirs
            bsp_led
            bsp_exio
            bsp_i2c
)

# 头文件
set(include_dirs
            bsp_led
            bsp_exio
            bsp_i2c
)

# 依赖项
set(requires
            driver
            common
            middlewares
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
