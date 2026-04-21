# 源文件
set(src_dirs
            src
)

# 头文件
set(include_dirs
            inc
)

# 依赖项
set(requires
            bsp
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
