# 源文件
set(src_dirs
            stumb
)

# 头文件
set(include_dirs
            stumb
)

# 依赖项
set(requires
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
