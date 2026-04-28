# 头文件
set(include_dirs
        inc
)

# 依赖项
set(requires
        middlewares
)

idf_component_register(
            INCLUDE_DIRS ${include_dirs}
            REQUIRES ${requires}
)