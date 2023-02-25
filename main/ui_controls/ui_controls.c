//
// Created by ilia on 24.2.23..
//
#include "ui_controls.h"
#include "encoder.h"
#include "button.h"

void changeScreen (lv_obj_t* screen){
    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, false);
}



void ui_updateStartScreen (uint32_t event){

    if (event & ENCODER_NOTIFY_MASK) {
        int pulseCount = (int) (event & 0x7F);
        if (event & 0x80) { pulseCount *= -1; }
        uint16_t max = lv_roller_get_option_cnt(ui_modeRoller) - 1;
        uint16_t current = lv_roller_get_selected(ui_modeRoller);

        lv_roller_set_selected(ui_modeRoller,
                               encoderCalculatePosition(pulseCount, current, 0, max),
                               LV_ANIM_OFF);
    }

    if (event & BTN_NOTIFY_MASK) {
        uint8_t btn = (event>>8) & 0b111;
        printf("BTN %d\n", btn);
        if (btn == 2) {
            if (lv_disp_get_scr_prev(NULL) != NULL)
                changeScreen(lv_disp_get_scr_prev(NULL));
            return;
        } else if (btn == 1) {
            switch (lv_roller_get_selected(ui_modeRoller)) {
                case 0:
                    changeScreen(ui_mixerMenuScreen);
                    break;
                case 1:
                    changeScreen(ui_tracklistScreen);
                    break;
                case 2:
                    changeScreen(ui_recPlayScreen);

                    break;
                case 3:
                    changeScreen(ui_mixerMenuScreen);
                    break;
                case 4:
                    changeScreen(ui_cableTestScreen);
                    break;
                case 5:
                    changeScreen(ui_optionsScreen);
                    break;
                default:
                    changeScreen(ui_startScreen);
                    break;

            }
        }
    }

}