//
// Created by ilia on 24.2.23..
//
#include "ui_controls.h"
#include "encoder.h"
#include "button.h"

void (*currentScreenUpd) (int, button_t*);

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
                    currentScreenUpd = ui_updateMixerScreen;
                    break;
                case 1:
                    changeScreen(ui_tracklistScreen);
                    //currentScreenUpd = ui_updateTrackListScreen;
                    break;
                case 2:
                    changeScreen(ui_recPlayScreen);
                    //currentScreenUpd = ui_updateRecScreen;
                    break;
                case 3:
                    changeScreen(ui_mixerMenuScreen);
                    //currentScreenUpd = ui_updateWaveGenScreen;
                    break;
                case 4:
                    changeScreen(ui_cableTestScreen);
                   // currentScreenUpd = ui_updateCableTestScreen;
                    break;
                case 5:
                    changeScreen(ui_optionsScreen);
                    //currentScreenUpd = ui_updateOptionsScreen;
                    break;
                default:

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
        count+=33;
        if (count>100) count = 0;
    }
}

void ui_updateMixerScreen (int encoder_delta, button_t* button_event) {

    lv_obj_t* objects[] = {NULL,ui_mixer_slider, ui_ouputLevelArc, ui_mic_line_switch, ui_hp_spk_switch};
    static uint8_t i_focused = 0; // focused obj index in array
    uint16_t val=0; // helper value

    if (encoder_delta) {
        if (objects[i_focused] != NULL)
        {
            if (lv_obj_get_state(objects[i_focused]) & LV_STATE_PRESSED)
            {
                if (objects[i_focused] == ui_mixer_slider) {
                    val = encoderCalculatePosition(encoder_delta, lv_slider_get_value(ui_mixer_slider), 0, 100);
                    lv_slider_set_value(ui_mixer_slider, val, LV_ANIM_OFF);
                    // set input gain
                    //update db label

                } else if (objects[i_focused] == ui_ouputLevelArc) {
                    val = encoderCalculatePosition(encoder_delta, lv_arc_get_value(objects[i_focused]), 0, 100);
                    lv_arc_set_value(objects[i_focused], val);
                    // set out gain here
                }
            }else {
                lv_obj_clear_state(objects[i_focused], LV_STATE_FOCUSED);
                i_focused = encoderCalculatePosition(encoder_delta, i_focused, 0, 4);
                if (objects[i_focused] != NULL) lv_obj_add_state(objects[i_focused], LV_STATE_FOCUSED);
            }
        } else {
            i_focused = encoderCalculatePosition(encoder_delta, i_focused, 0, 4);
            if (objects[i_focused] != NULL) lv_obj_add_state(objects[i_focused], LV_STATE_FOCUSED);
        }
    }

    if (button_event != NULL) {
        if (objects[i_focused] != NULL) {
            if (objects[i_focused] == ui_mixer_slider || objects[i_focused] == ui_ouputLevelArc)
            {
                if (button_event->pin == BTN_ENC_PIN) {
                    lv_obj_add_state(objects[i_focused], LV_STATE_PRESSED);
                } else if (button_event->pin == BTN_LEFT_PIN) {
                    lv_obj_clear_state(objects[i_focused], LV_STATE_PRESSED);
                }
            } else if (button_event->pin == BTN_ENC_PIN) {
                if (lv_obj_get_state(objects[i_focused]) & LV_STATE_CHECKED) {
                    lv_obj_clear_state(objects[i_focused], LV_STATE_CHECKED);
                    if (objects[i_focused] == ui_hp_spk_switch) {
                        // codec disable spk
                    } else if (objects[i_focused] == ui_mic_line_switch) {
                        // codec select input channel mic
                    }
                } else {
                    lv_obj_add_state(objects[i_focused], LV_STATE_CHECKED);
                    if (objects[i_focused] == ui_hp_spk_switch) {
                        // codec enable spk
                    } else if (objects[i_focused] == ui_mic_line_switch) {
                        // codec select input channel line
                    }
                }
            }
        } //if (objects[i_focused] != NULL)
        if (button_event->pin == BTN_LEFT_PIN && button_event->wasLong) {
            changeScreen(ui_startScreen);
            currentScreenUpd = ui_updateStartScreen;
            // mute codec
            //
        }
    } // if button event

    // to do : update vu bar from codec data
    //
    //
}