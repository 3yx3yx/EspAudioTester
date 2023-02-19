idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS ""
        INCLUDE_DIRS "../components/st7789"
        SRCS "../components/st7789/st7789.c"
        SRCS "../components/st7789/disp_spi.c"
        )

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
