idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS ""
        INCLUDE_DIRS "../components/st7789"
        INCLUDE_DIRS "../components/ui"
        SRCS "../components/st7789/st7789.c"
        SRCS "../components/st7789/disp_spi.c"

        SRCS "../components/ui/ui.c"
        SRCS "../components/ui/ui_comp.c"
        SRCS "../components/ui/ui_helpers.c"
        SRCS "../components/ui/ui_img_accept_png.c"
        SRCS "../components/ui/ui_img_headphone_png.c"
        SRCS "../components/ui/ui_img_indicator_ver_png.c"
        SRCS "../components/ui/ui_img_mic_png.c"
        SRCS "../components/ui/ui_img_mixer_png.c"
        SRCS "../components/ui/ui_img_pause_png.c"
        SRCS "../components/ui/ui_img_play_png.c"
        SRCS "../components/ui/ui_img_pot_ver_knob_png.c"
        SRCS "../components/ui/ui_img_rec_png.c"
        SRCS "../components/ui/ui_img_speaker_png.c"
        SRCS "../components/ui/ui_img_stop_png.c"
        SRCS "../components/ui/ui_img_xlr_png.c"

        INCLUDE_DIRS "./encoder"
        SRCS "./encoder/encoder.c"

        INCLUDE_DIRS "./button"
        SRCS "./button/button.c"

        INCLUDE_DIRS "./ui_controls"
        SRCS "./ui_controls/ui_controls.c"

        INCLUDE_DIRS "./codec"
        SRCS "./codec/wm8960.c"
        SRCS "./codec/sd_card.c"
        SRCS "./codec/wav.c"

        INCLUDE_DIRS "./"
        )

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
