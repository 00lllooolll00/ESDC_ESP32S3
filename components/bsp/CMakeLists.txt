# 源文件
set(bsps
            bsp_i2c
            bsp_i2s
            bsp_spi
            bsp_led
            bsp_exio
            bsp_rgblcd
            bsp_touch
            bsp_spilcd
            bsp_key
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
            fatfs

            common
            middlewares
)

# 注册到 idf
idf_component_register(
            SRC_DIRS ${bsps}
            INCLUDE_DIRS ${bsps}
            REQUIRES ${requires}
)

# 当前组件的编译选项
component_compile_options(
)
