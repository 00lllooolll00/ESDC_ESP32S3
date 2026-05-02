# 源文件
set(src_dirs
            bsp_led
            bsp_exio
            bsp_i2c
            bsp_lcd
            bsp_key
            bsp_wifi
            bsp_audio
            bsp_i2s
)

# 头文件
set(include_dirs
            bsp_led
            bsp_exio
            bsp_i2c
            bsp_lcd
            bsp_key
            bsp_wifi
            bsp_audio
            bsp_i2s
)

# 依赖项
set(requires
            esp_driver_gpio
            esp_driver_i2s
            esp_driver_i2c
            esp_driver_spi
            esp_lcd
            esp_psram
            esp_event
            esp_wifi
            
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
