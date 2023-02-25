//
// Created by ilia on 24.2.23..
//
#include "ui_controls.h"
#include "encoder.h"
#include "button.h"

void (*currentScreenUpd) (int encoder_delta, button_t* button_event);

void changeScreen (lv_obj_t* screen){
    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, false);
}

void ui_updateStartScreen (int encoder_delta, button_t* button_event){

    if (encoder_delta) {
        uint16_t max = lv_roller_get_option_cnt(ui_start_menu_roller) - 1;
        uint16_t current = lv_roller_get_selected(ui_start_menu_roller);
        lv_roller_set_selected(ui_start_menu_roller,
                               encoderCalculatePosition(encoder_delta, current, 0, max),
                               LV_ANIM_OFF);
    }

    if (button_event != NULL) {
        if (button_event->pin==BTN_LEFT_PIN) {
            if (lv_disp_get_scr_prev(NULL) != NULL)
                changeScreen(lv_disp_get_scr_prev(NULL));
            return;
        } else if (button_event->pin==BTN_ENC_PIN) {
            switch (lv_roller_get_selected(ui_start_menu_roller)) {
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

void ui_set_battery_bar (int charge) {
    if (charge <= 100) {
        char charge_lable[5];
        sprintf(charge_lable,"%d%%", charge);
        lv_label_set_text(ui_battery_val, charge_lable);
        lv_bar_set_value(ui_battery_bar, charge, LV_ANIM_OFF);
    }
    else { // if charger is plugged run animation
        lv_label_set_text(ui_battery_val," ");
        static uint8_t count = 0;
        lv_bar_set_value(ui_battery_bar, count, LV_ANIM_OFF);
        count+=20;
        if (count>100) count = 0;
    }
}