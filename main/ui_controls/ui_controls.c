//
// Created by ilia on 24.2.23..
//
#include "ui_controls.h"
#include "encoder.h"
#include "button.h"
#include "sd_card.h"
#include "wav.h"

#define PRESS(obj) lv_obj_add_state(obj, LV_STATE_PRESSED);
#define UNPRESS(obj) lv_obj_clear_state(obj, LV_STATE_PRESSED);
#define FOCUS(obj)  lv_obj_add_state(obj, LV_STATE_FOCUSED);
#define UNFOCUS(obj)  lv_obj_clear_state(obj, LV_STATE_FOCUSED);
#define CHECK_OBJ(obj) lv_obj_add_state(obj, LV_STATE_CHECKED);
#define UNCHECK_OBJ(obj) lv_obj_clear_state(obj, LV_STATE_CHECKED);

/**
 * @brief function pointer to current screen update function
 * @param int encoder ticks delta
 * @param button_t* pointer to button event descriptor struct
 */
void (*currentScreenUpd) (int, button_t*);
void (*prevScreenUpd) (int, button_t*);

#define TRACK_NUM_MAX 10
static int file_selected_pos= 0;

void changeScreen (lv_obj_t* screen){
    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, false);
}

static void openMixerMenu(void);
static void openWaveGenMenu (void);
static void openRecMenu (void);
static void openPlayMenu (void);
static void openTracklistMenu (void);
static void openAcceptDeclineMenu (void);
static void openOptionsMenu (void);
static void openCableTestMenu (void);



static bool escapeButtonEvent (button_t* button_event);

enum movableObjects{
    ROLLER,
    SLIDER,
    ARC
};
static void incrementObj (uint8_t type, lv_obj_t* obj, int inc) {
    int min,max,current,val;

    switch (type) {
        case ROLLER:
            min=0;
            max = lv_roller_get_option_cnt(obj) - 1;
            current = lv_roller_get_selected(obj);
            val= encoderCalculatePosition(inc, current, min, max);
            lv_roller_set_selected(obj,val,LV_ANIM_OFF);
            break;
        case SLIDER:
            min = lv_slider_get_min_value(obj);
            max = lv_slider_get_max_value(obj);
            current = lv_slider_get_value(obj);
            val = encoderCalculatePosition(inc,current,min,max);
            lv_slider_set_value(obj, val, LV_ANIM_OFF);
            break;
        case ARC:
            min = lv_arc_get_min_value(obj);
            max = lv_arc_get_max_value(obj);
            current = lv_arc_get_value(obj);
            val = encoderCalculatePosition(inc,current,min,max);
            lv_arc_set_value(obj, val);
            break;
    }
}

static void openMixerMenu(void) {                                    //mixer
    changeScreen(ui_mixerMenuScreen);
    currentScreenUpd = ui_updateMixerScreen;
    lv_obj_clear_flag(ui_mic_line_switch, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_mic_icon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_spkIcon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_hp_spk_switch, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_xlr_icon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_hpIcon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_mixer_slider, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_mixer_db_val, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui_mixer_arc_label, "monitor");
}
static void openPlayerMixerMenu(void) {                                    //mixer player
    changeScreen(ui_mixerMenuScreen);
    currentScreenUpd = ui_updatePlayerMixer;
    lv_obj_add_flag(ui_mic_line_switch, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_mic_icon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_spkIcon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_hp_spk_switch, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_xlr_icon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_hpIcon, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(ui_mixer_slider, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_mixer_db_val, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui_mixer_arc_label, "level");
}
static void openWaveGenMenu (void) {                                      //wave
    changeScreen(ui_mixerMenuScreen);
    currentScreenUpd = ui_updateWaveGenScreen;
    lv_obj_add_flag(ui_mic_line_switch, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_mic_icon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_spkIcon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_hp_spk_switch, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_xlr_icon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_hpIcon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_mixer_slider, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_mixer_db_val, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui_mixer_arc_label, "freq");
}

lv_chart_series_t * chartSeries;

static void openRecMenu (void){                                       //record
    changeScreen(ui_recPlayScreen);
    currentScreenUpd = ui_updateRecScreen;
    lv_img_set_src(ui_rec_play_right_button, &ui_img_rec_png );
    lv_label_set_text(ui_elapsed_time_label, "");
    lv_label_set_text(ui_rec_play_track_len_label, "");
    lv_slider_set_value(ui_timePosSlider,0, LV_ANIM_OFF);
    if (chartSeries!=NULL){
        lv_chart_remove_series(ui_chart, chartSeries);
    }
    lv_chart_set_point_count(ui_chart, 100);
    lv_chart_set_type(ui_chart, LV_CHART_TYPE_BAR);
    chartSeries = lv_chart_add_series(ui_chart, lv_color_white(), LV_CHART_AXIS_PRIMARY_Y);

}
static void openPlayMenu (void){                                   //play
    changeScreen(ui_recPlayScreen);
    currentScreenUpd = ui_updatePlayScreen;
    lv_img_set_src(ui_rec_play_right_button, &ui_img_play_png );
    lv_label_set_text(ui_elapsed_time_label, " ");
    lv_label_set_text(ui_rec_play_track_len_label, "0.00");
    lv_slider_set_value(ui_timePosSlider,0, LV_ANIM_OFF);

    if (chartSeries!=NULL){
        lv_chart_remove_series(ui_chart, chartSeries);
    }
    lv_chart_set_point_count(ui_chart, 100);
    lv_chart_set_type(ui_chart, LV_CHART_TYPE_BAR);
    chartSeries = lv_chart_add_series(ui_chart, lv_color_white(), LV_CHART_AXIS_PRIMARY_Y);


    char fname [255] = "";
    get_nth_file_name(file_selected_pos,fname);
    uint32_t wav_size = wav_get_size(fname);

    if (wav_size == 0) { // if cannot read the file
        lv_label_set_text(ui_elapsed_time_label, "ERROR");
        lv_label_set_text(ui_rec_play_track_len_label, "ERROR");
        return;
    }

    // draw the audio amplitude diagram
    FILE* f = fopen(fname,"r");
    fseek(f, WAVE_HEADER_SIZE, SEEK_SET);
    uint16_t smooth_buf[5] = {};
    uint8_t smooth_buf_idx=0;
    for (int i = 0; i < 100; i++) {
        uint16_t peak = 0;
        fread(&peak, sizeof(uint16_t), 1, f);
        peak /= (UINT16_MAX) / 100;

        //smoothing
        smooth_buf[smooth_buf_idx] = peak;
        smooth_buf_idx++;
        if (smooth_buf_idx > 4) { smooth_buf_idx = 0; }
        peak = 0;
        for (int j = 0; j < 5; j++) { peak += smooth_buf[j]; }
        peak /= 5;

        chartSeries->y_points[i] = peak;

        fseek(f, (wav_size / 100), SEEK_CUR);
    }
    lv_chart_refresh(ui_chart);
    fclose(f);

    // calculate and show track duration
    uint32_t track_len_sec = wav_size /  BYTE_RATE;
    char duration_label [10] = "";
    sprintf(duration_label, "%d:%d", track_len_sec/60, track_len_sec % 60);
    lv_label_set_text(ui_rec_play_track_len_label, duration_label);

}
static void openTracklistMenu (void){

    char list[350] = "";
    sd_get_file_list(list, file_selected_pos, TRACK_NUM_MAX + file_selected_pos);
    if (strlen(list)) {
        lv_roller_set_options(ui_trackListRoller, list, LV_ROLLER_MODE_NORMAL);
        lv_roller_set_selected(ui_trackListRoller, 0, LV_ANIM_OFF);
    } else { // if no files found go to 0 position in list
        file_selected_pos=0;
        sd_get_file_list(list, file_selected_pos, TRACK_NUM_MAX + file_selected_pos);
        lv_roller_set_selected(ui_trackListRoller, 0, LV_ANIM_OFF);
        lv_roller_set_options(ui_trackListRoller, list, LV_ROLLER_MODE_NORMAL);
    }

    if (lv_disp_get_scr_act(NULL) != ui_tracklistScreen) {
        changeScreen(ui_tracklistScreen);
        currentScreenUpd = ui_updateTrackListScreen;
    }
}
static void openAcceptDeclineMenu (void){
    changeScreen(ui_acceptDeclineScreen);
    currentScreenUpd = ui_updateAcceptDeclineScreen;
}
static void openOptionsMenu (void) {
    changeScreen(ui_optionsScreen);
    currentScreenUpd = ui_updateOptionsScreen;
}
static void openCableTestMenu (void){
    changeScreen(ui_cableTestScreen);
    currentScreenUpd = ui_updateCableTestScreen;
}

static bool escapeButtonEvent (button_t* button_event){
    if (button_event == NULL) return false;
    if (button_event->pin == BTN_LEFT_PIN && button_event->wasLong) {
        changeScreen(ui_startScreen);
        currentScreenUpd = ui_updateStartScreen;
        return true;
    }
    return false;
}


void ui_updateStartScreen (int encoder_delta, button_t* button_event){

    if (encoder_delta) {
        incrementObj(ROLLER,ui_start_menu_roller,encoder_delta);
    }

    if (button_event != NULL && button_event->pin == BTN_ENC_PIN) {
        switch (lv_roller_get_selected(ui_start_menu_roller)) {
            case 0: openMixerMenu();break;
            case 1: openTracklistMenu();break;
            case 2: openRecMenu();break;
            case 3: openWaveGenMenu();break;
            case 4: openCableTestMenu();break;
            case 5: openOptionsMenu();break;
            default: break;
        }
    }

}
/**
 * @brief update battery bar
 * @param charge percents. if > 100 animation will start
 */
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

void ui_updateMixerScreen(int encoder_delta, button_t *button_event) {

    lv_obj_t *objects[] = {NULL, ui_mixer_slider, ui_ouputLevelArc, ui_mic_line_switch, ui_hp_spk_switch};
    static uint8_t i_focused = 0; // focused obj index in array
    uint8_t nObj = 4;
    bool waveGenMode = 0;
    if (currentScreenUpd == ui_updateWaveGenScreen) { nObj = 2; waveGenMode = 1; } // in wave generator mode switches are hidden

    if (encoder_delta) {
        if (objects[i_focused] != NULL) {
            if (lv_obj_get_state(objects[i_focused]) & LV_STATE_PRESSED) {
                if (objects[i_focused] == ui_mixer_slider) {
                    incrementObj(SLIDER, objects[i_focused], encoder_delta);
                    if (waveGenMode) {
                        //update generator output
                        //
                    } else {
                        //update input gain
                        //
                    }
                } else if (objects[i_focused] == ui_ouputLevelArc) {
                    incrementObj(ARC, objects[i_focused], encoder_delta);
                    if (waveGenMode) {
                        //update generator freq
                        //
                    } else {
                        //update output level
                        //
                    }
                }
            } else { // if no pressed objects, change focused object
                UNFOCUS(objects[i_focused]);
                i_focused = encoderCalculatePosition(encoder_delta, i_focused, 0, nObj);
                if (objects[i_focused] != NULL) FOCUS(objects[i_focused]);
            }
        } else {
            i_focused = encoderCalculatePosition(encoder_delta, i_focused, 0, nObj);
            if (objects[i_focused] != NULL) FOCUS(objects[i_focused]);
        }
    }

    if (button_event != NULL) {

        if (button_event->pin == BTN_LEFT_PIN && lv_disp_get_scr_prev(NULL)!=ui_recPlayScreen){ // left btn => esc to rec menu
            if (objects[i_focused] != NULL) {
                if (!((lv_obj_get_state(objects[i_focused]) & LV_STATE_PRESSED))) openRecMenu();
            } else openRecMenu();
        }

        if (objects[i_focused] != NULL) {
            if (objects[i_focused] == ui_mixer_slider || objects[i_focused] == ui_ouputLevelArc)
            {
                if (button_event->pin == BTN_ENC_PIN) {
                    PRESS(objects[i_focused]);
                } else if (button_event->pin == BTN_LEFT_PIN) {
                    UNPRESS(objects[i_focused]);
                }
            } else if (button_event->pin == BTN_ENC_PIN && !waveGenMode) {
                if (lv_obj_get_state(objects[i_focused]) & LV_STATE_CHECKED) {
                    UNCHECK_OBJ(objects[i_focused]);
                    if (objects[i_focused] == ui_hp_spk_switch) {
                        // codec disable spk
                        //
                    } else if (objects[i_focused] == ui_mic_line_switch) {
                        // if mic selected disable speaker
                        UNCHECK_OBJ(ui_hp_spk_switch);
                        // codec select input channel mic
                        //
                    }
                } else {
                    CHECK_OBJ(objects[i_focused]);
                    if (objects[i_focused] == ui_hp_spk_switch) {
                        // select xlr input if mic was selected
                        CHECK_OBJ(ui_mic_line_switch);
                        // codec enable spk
                        //
                    } else if (objects[i_focused] == ui_mic_line_switch) {
                        // codec select input channel line
                        //
                    }
                }
            }
        } //if (objects[i_focused] != NULL)
        if (escapeButtonEvent(button_event)) {
            // mute codec
            //
        }
    } // if button event

    // to do : update vu bar from codec data
    //
    //
}

void ui_updateWaveGenScreen (int encoder_delta, button_t* button_event){
    ui_updateMixerScreen (encoder_delta, button_event); // wavegen and mixer has the same ui objects
}

void ui_updateTrackListScreen (int encoder_delta, button_t* button_event){
    if (button_event != NULL) {
        if (button_event->pin == BTN_ENC_PIN )openPlayMenu();
        //play selected track
    }

    if (encoder_delta) {
        file_selected_pos+=encoder_delta;
        if (file_selected_pos<0) {
            file_selected_pos=0;
        }
        if (file_selected_pos > get_files_count_at_mp()-1) {
            file_selected_pos = get_files_count_at_mp()-1;
        }

        lv_obj_t* roller = ui_trackListRoller;
        if (((lv_roller_get_selected(roller) == lv_roller_get_option_cnt(roller)-1) && encoder_delta > 0)
            || ((lv_roller_get_selected(roller) == 0) && encoder_delta < 0)) {
            openTracklistMenu(); // reload files list if out of roller range
            return;
        }
        incrementObj(ROLLER, ui_trackListRoller, encoder_delta);
    }

    if (escapeButtonEvent(button_event)) { /*go to menu*/}
}

void ui_updateRecScreen (int encoder_delta, button_t* button_event){
    static bool started = 0;
    static bool paused = 0;

    if (button_event != NULL) {
        if (button_event->pin == BTN_ENC_PIN) {
            if (!started) {
                started = true;
                lv_img_set_src(ui_rec_play_right_button, &ui_img_pause_png);
                lv_img_set_src(ui_rec_play_left_button, &ui_img_stop_png);
                // start recording
                //
            }else if (started && !paused) {
                paused = true;
                lv_img_set_src(ui_rec_play_right_button, &ui_img_rec_png);
                lv_img_set_src(ui_rec_play_left_button, &ui_img_mixer_png);
                //pause record
                //
            } else if (started && paused) {
                paused = false;
                lv_img_set_src(ui_rec_play_right_button, &ui_img_pause_png);
                lv_img_set_src(ui_rec_play_left_button, &ui_img_stop_png);
                // resume record
                //
            }
        }

        if (button_event->pin == BTN_LEFT_PIN) {
            if (lv_img_get_src(ui_rec_play_left_button) == &ui_img_mixer_png) {
                openMixerMenu();
            } else { openAcceptDeclineMenu();}
        }

        if (escapeButtonEvent(button_event)) {
            // mute codec
            //
        }
    }

}

void ui_updatePlayScreen (int encoder_delta, button_t* button_event){

    static bool paused = 0;

    if (button_event != NULL) {
        if (button_event->pin == BTN_ENC_PIN) {
            if (!paused) {
                paused = true;
                lv_img_set_src(ui_rec_play_right_button, &ui_img_pause_png);
                // start playing
                //
                char fname [255] = "";
                get_nth_file_name(file_selected_pos,fname);
                wav_open_file(fname);
            }else {
                paused = false;
                lv_img_set_src(ui_rec_play_right_button, &ui_img_play_png);
                // pause
                //
            }
        }

        if (button_event->pin == BTN_LEFT_PIN) {
            openPlayerMixerMenu();
        }

        if (escapeButtonEvent(button_event)) {
            // mute codec
            //
        }
    }

}

void ui_updatePlayerMixer (int encoder_delta, button_t* button_event) {

    lv_obj_t *objects[] = {NULL, ui_ouputLevelArc,ui_hp_spk_switch};
    static uint8_t i_focused = 0; // focused obj index in array
    uint8_t nObj = 2;

    if (encoder_delta) {
        if (objects[i_focused] != NULL) {
            if (lv_obj_get_state(objects[i_focused]) & LV_STATE_PRESSED) {
                if (objects[i_focused] == ui_ouputLevelArc) {
                    incrementObj(ARC, objects[i_focused], encoder_delta);
                    //update output level
                    //
                }
            } else { // if no pressed objects, change focused object
                UNFOCUS(objects[i_focused]);
                i_focused = encoderCalculatePosition(encoder_delta, i_focused, 0, nObj);
                if (objects[i_focused] != NULL) FOCUS(objects[i_focused]);
            }
        } else {
            i_focused = encoderCalculatePosition(encoder_delta, i_focused, 0, nObj);
            if (objects[i_focused] != NULL) FOCUS(objects[i_focused]);
        }
    }

    if (button_event != NULL) {

        if (button_event->pin == BTN_LEFT_PIN){ // left btn => esc to play menu
            if (objects[i_focused] != NULL) {
                if (!((lv_obj_get_state(objects[i_focused]) & LV_STATE_PRESSED))) openPlayMenu(); // if arc is not pressed
            } else openPlayMenu();
        }

        if (objects[i_focused] != NULL) {
            if (objects[i_focused] == ui_ouputLevelArc)
            {
                if (button_event->pin == BTN_ENC_PIN) {
                    PRESS(objects[i_focused]);
                } else if (button_event->pin == BTN_LEFT_PIN) {
                    UNPRESS(objects[i_focused]);
                }
            } else if (button_event->pin == BTN_ENC_PIN) {
                if (lv_obj_get_state(objects[i_focused]) & LV_STATE_CHECKED) {
                    UNCHECK_OBJ(objects[i_focused]);
                    if (objects[i_focused] == ui_hp_spk_switch) {
                        // codec disable spk
                        //
                    }
                } else {
                    CHECK_OBJ(objects[i_focused]);
                    if (objects[i_focused] == ui_hp_spk_switch) {
                        // codec enable spk
                        //
                    }
                }
            }
        } //if (objects[i_focused] != NULL)

        if (escapeButtonEvent(button_event)) {
            // mute codec
            //
        }
    } // if button event

    // to do : update vu bar from codec data
    //
    //
}

void ui_updateAcceptDeclineScreen (int encoder_delta, button_t* button_event){

}

void ui_updateOptionsScreen (int encoder_delta, button_t* button_event){

}

void ui_updateCableTestScreen (int encoder_delta, button_t* button_event){

}
