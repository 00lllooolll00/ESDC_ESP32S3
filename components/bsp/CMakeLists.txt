# 源文件
set(src_dirs
            bsp_i2c
            bsp_i2s
            bsp_spi
            bsp_led
            bsp_exio
            bsp_lcd
            bsp_key
            bsp_wifi
            bsp_audio
            bsp_sdcard
)

# 头文件
set(include_dirs
            bsp_i2c
            bsp_i2s
            bsp_spi
            bsp_led
            bsp_exio
            bsp_lcd
            bsp_key
            bsp_wifi
            bsp_audio
            bsp_sdcard
)

# 依赖项
set(requires
            esp_driver_gpio
            esp_driver_i2s
            esp_driver_i2c
            esp_driver_spi
            esp_driver_sdspi
            esp_lcd
            esp_psram
            esp_event
            esp_wifi
            fatfs            

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
