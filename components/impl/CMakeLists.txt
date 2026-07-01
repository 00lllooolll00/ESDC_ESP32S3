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
        common
        platform
        bsp
        middlewares
        esp_wifi
        esp_netif
        mqtt
        esp-sr
)
idf_component_register(
            SRC_DIRS ${src_dirs}
            INCLUDE_DIRS ${include_dirs}
            REQUIRES ${requires}
            WHOLE_ARCHIVE
)
