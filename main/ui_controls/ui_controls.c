//
// Created by ilia on 24.2.23..
//
#include "ui_controls.h"
#include "encoder.h"
#include "button.h"
#include "sd_card.h"
#include "wav.h"
#include "wm8960.h"

#define PRESS(obj) lv_obj_add_state(obj, LV_STATE_PRESSED)
#define UNPRESS(obj) lv_obj_clear_state(obj, LV_STATE_PRESSED)
#define FOCUS(obj)  lv_obj_add_state(obj, LV_STATE_FOCUSED)
#define UNFOCUS(obj)  lv_obj_clear_state(obj, LV_STATE_FOCUSED)
#define CHECK_OBJ(obj) lv_obj_add_state(obj, LV_STATE_CHECKED)
#define UNCHECK_OBJ(obj) lv_obj_clear_state(obj, LV_STATE_CHECKED)

#define IS_PRESSED(obj) (lv_obj_get_state(obj) & LV_STATE_PRESSED)
#define IS_CHECKED(obj) (lv_obj_get_state(obj) & LV_STATE_CHECKED)
#define IS_FOCUSED(obj) (lv_obj_get_state(obj) & LV_STATE_FOCUSED)

/**
 * @brief function pointer to current screen update function
 * @param int encoder ticks delta
 * @param button_t* pointer to button event descriptor struct
 */
void (*currentScreenUpd) (int, button_t*);
void (*prevScreenUpd) (int, button_t*);

#define TRACK_NUM_MAX 10
static int file_selected_pos= 0;
static int file_selected_pos_prev= -1;
uint32_t track_len_sec = 0;
uint32_t track_elapsed_sec = 0;
extern TaskHandle_t wav_task_handle;
bool started = 0;

static uint8_t input_gain_slider_val = 50;
static uint8_t line_out_arc_val = HP_LINE_VOL_DEFAULT;
static uint8_t monitor_arc_val = HP_LINE_VOL_DEFAULT;
static uint8_t wave_gen_slider_val = HP_LINE_VOL_DEFAULT;
static uint8_t wave_gen_freq_arc_val = 50;

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



static bool checkEscapeButtonEvent (button_t* button_event);

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

    xTaskNotify(wav_task_handle,MONITOR_START,eSetValueWithOverwrite);
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

    //lv_obj_add_flag(ui_mixer_slider, LV_OBJ_FLAG_HIDDEN);
    //lv_obj_add_flag(ui_mixer_db_val, LV_OBJ_FLAG_HIDDEN);
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


    lv_label_set_text(ui_line_out_label, "freq");
    lv_label_set_text(ui_mixer_slider_func_label, "out");
    lv_obj_add_flag(ui_ouputLevelArc, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_mixer_arc_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_mixer_arc_value, LV_OBJ_FLAG_HIDDEN);

    xTaskNotify(wav_task_handle,WAVEGEN_START,eSetValueWithOverwrite);

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

static void increment_elapsed_time (int inc) {
    char s [10] = "";
    track_elapsed_sec += inc;

    if (track_elapsed_sec > track_len_sec) track_elapsed_sec =0;

    if ((track_elapsed_sec % 60) < 10) { // add a zero for better look
        sprintf(s, "%d:0%d", track_elapsed_sec/60, track_elapsed_sec % 60);
    } else {
        sprintf(s, "%d:%d", track_elapsed_sec/60, track_elapsed_sec % 60);
    }

    lv_label_set_text(ui_elapsed_time_label, s);

    int pos = ((double)track_elapsed_sec/track_len_sec)*100;

    lv_slider_set_value(ui_timePosSlider, pos, LV_ANIM_OFF);
}

static bool file_error = 0;

static void openPlayMenu (void){                                   //play
    changeScreen(ui_recPlayScreen);
    currentScreenUpd = ui_updatePlayScreen;

    if (file_selected_pos == file_selected_pos_prev) {
        if (!started){
            lv_img_set_src(ui_rec_play_right_button, &ui_img_play_png );
            track_elapsed_sec = 0;
            lv_slider_set_value(ui_timePosSlider,0, LV_ANIM_OFF);
            lv_label_set_text(ui_elapsed_time_label, "0.00");
        } // if track opened from tracklist
        return;
    }

    file_selected_pos_prev = file_selected_pos;

    track_elapsed_sec = 0;

    lv_arc_set_value(ui_ouputLevelArc, HP_LINE_VOL_DEFAULT);
    lv_obj_clear_state(ui_hp_spk_switch,LV_STATE_CHECKED); // speaker is muted initially
    lv_label_set_text(ui_mixer_arc_value,"-2.0");
    lv_slider_set_value(ui_mixer_slider,DAC_VOL_DEFAULT,LV_ANIM_OFF);
    lv_label_set_text(ui_mixer_db_val,"-0.0");

    lv_img_set_src(ui_rec_play_right_button, &ui_img_play_png );
    lv_img_set_src(ui_rec_play_left_button, &ui_img_mixer_png );
    lv_label_set_text(ui_elapsed_time_label, "0.00");
    lv_label_set_text(ui_rec_play_track_len_label, "0.00");
    lv_slider_set_value(ui_timePosSlider,0, LV_ANIM_OFF);

    if (chartSeries!=NULL){
        lv_chart_remove_series(ui_chart, chartSeries);
    }
    lv_chart_set_point_count(ui_chart, 100);
    lv_chart_set_type(ui_chart, LV_CHART_TYPE_BAR);
    chartSeries = lv_chart_add_series(ui_chart, lv_color_white(), LV_CHART_AXIS_PRIMARY_Y);

    file_error=0;
    char fname [255] = "";
    get_nth_file_name(file_selected_pos,fname);

    uint32_t wav_size = wav_get_size(fname);
    uint8_t n_channels = wav_curr_file_get_n_channels();

    if (wav_size == 0) { // if cannot read the file
        lv_label_set_text(ui_elapsed_time_label, "ERROR");
        lv_label_set_text(ui_rec_play_track_len_label, "ERROR");
        file_error = true;
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
    if (n_channels == 2) {
        track_len_sec = wav_size / BYTE_RATE_44100;
    } else {
        track_len_sec = (wav_size*2) / BYTE_RATE_44100;
    }

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

static bool checkEscapeButtonEvent (button_t* button_event){
    if (button_event == NULL) return false;
    if (button_event->pin == BTN_LEFT_PIN && button_event->wasLong) {
        changeScreen(ui_startScreen);
        currentScreenUpd = ui_updateStartScreen;
        return true;
    }
    return false;
}

static bool first_scr_upd = true;

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

    started = false;
    xTaskNotify(wav_task_handle,AUDIO_STOP,eSetValueWithOverwrite);
    first_scr_upd = true;

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

    static lv_obj_t * cur_obj = NULL;
    static int i_obj_focused = 0;
    const int n_objects  = 4;
    bool need_codec_upd = first_scr_upd;
    first_scr_upd = false;

    if (encoder_delta) {
        bool menu_scroll = false;
        if (cur_obj != NULL) {
            printf ("obj_i = %d\n", i_obj_focused);

            if (IS_PRESSED(cur_obj)) {
                if (cur_obj == ui_mixer_slider) {
                    int cur_val = lv_slider_get_value(ui_mixer_slider);
                    input_gain_slider_val = encoderCalculatePosition(encoder_delta, cur_val, 0, 100);
                } else if (cur_obj == ui_lineLevelArc) {
                    int cur_val = lv_arc_get_value(ui_lineLevelArc);
                    line_out_arc_val = encoderCalculatePosition(encoder_delta, cur_val, 0, 100);
                } else if (cur_obj == ui_ouputLevelArc){
                    int cur_val = lv_arc_get_value(ui_ouputLevelArc);
                    monitor_arc_val = encoderCalculatePosition(encoder_delta, cur_val, 0, 100);
                }

            } else {
                UNFOCUS(cur_obj);
                if (cur_obj == ui_ouputLevelArc) UNFOCUS(ui_lineLevelArc);
                menu_scroll = true;
            }
        }

        if (menu_scroll || cur_obj==NULL) {
            i_obj_focused = encoderCalculatePosition(encoder_delta, i_obj_focused, 0, n_objects);
            switch (i_obj_focused) {

                case 0:
                    cur_obj = NULL;
                    break;
                case 1:
                    cur_obj = ui_mixer_slider;
                    break;
                case 2:
                    cur_obj = ui_mic_line_switch;
                    break;
                case 3:
                    cur_obj = ui_lineLevelArc;
                    break;
                case 4:
                    cur_obj = ui_hp_spk_switch;
                    break;
                default:
                    break;

            }

            if (cur_obj != NULL) {
                FOCUS(cur_obj);
            }
        }


    }

    if (button_event != NULL) {

        if (button_event->pin == BTN_LEFT_PIN &&
            lv_disp_get_scr_prev(NULL) == ui_recPlayScreen) { // left btn => esc to rec menu
            if (cur_obj != NULL) {
                if (!IS_PRESSED(cur_obj)) {
                    openRecMenu();
                    return;
                }
            } else {
                openRecMenu();
                return;
            }
        }

        if (cur_obj != NULL) {
            if (cur_obj == ui_lineLevelArc) {

                if (button_event->pin == BTN_ENC_PIN) {
                    if (IS_PRESSED(ui_lineLevelArc)) {
                        UNPRESS(ui_lineLevelArc);
                        PRESS (ui_ouputLevelArc);
                        cur_obj = ui_ouputLevelArc;
                    } else {
                        PRESS(ui_lineLevelArc);
                    }
                } else if (button_event->pin == BTN_LEFT_PIN) {
                    UNPRESS(ui_lineLevelArc);
                }

            } else if (cur_obj == ui_ouputLevelArc) {
                if (button_event->pin == BTN_ENC_PIN) {
                    if (IS_PRESSED(ui_ouputLevelArc)) {
                        UNPRESS(ui_ouputLevelArc);
                        PRESS (ui_lineLevelArc);
                        cur_obj = ui_lineLevelArc;
                    } else {
                        PRESS(ui_ouputLevelArc);
                    }
                } else if (button_event->pin == BTN_LEFT_PIN) {
                    UNPRESS(ui_ouputLevelArc);
                }
            } else if (cur_obj == ui_mixer_slider) {
                if (button_event->pin == BTN_ENC_PIN) {
                    PRESS (ui_mixer_slider);
                } else {
                    UNPRESS(ui_mixer_slider);
                }
            } else if (cur_obj == ui_mic_line_switch) {
                if (button_event->pin == BTN_ENC_PIN) {
                    need_codec_upd = true;
                    bool checked = IS_CHECKED(ui_mic_line_switch);
                    if (checked) {
                        UNCHECK_OBJ(ui_mic_line_switch);
                        if (IS_CHECKED(ui_hp_spk_switch)) {
                            UNCHECK_OBJ(ui_hp_spk_switch);
                            //mute spk
                            codec_set_speaker_vol(0);
                        }
                        codec_set_line_in_gain(0);
                    } else {
                        codec_set_mic_gain(0);
                        CHECK_OBJ(ui_mic_line_switch);
                    }
                }
            } else if (cur_obj == ui_hp_spk_switch) {
                need_codec_upd = true;
                if (button_event->pin == BTN_ENC_PIN) {
                    if (IS_CHECKED(ui_hp_spk_switch)) {
                        UNCHECK_OBJ(ui_hp_spk_switch);
                        codec_set_speaker_vol(0);
                    } else {
                        CHECK_OBJ(ui_hp_spk_switch);
                        codec_set_hp_vol(0);
                        if (!IS_CHECKED(ui_mic_line_switch)) {
                            CHECK_OBJ(ui_mic_line_switch);
                            codec_set_mic_gain(0);
                        }
                    }
                }
            }
        }
    }

    char s [10] = "";
    float db = 0;

    if (input_gain_slider_val != lv_slider_get_value(ui_mixer_slider) || need_codec_upd) {
        if (IS_CHECKED(ui_mic_line_switch)) { //  line selected
            db = codec_set_line_in_gain(input_gain_slider_val);
            if (IS_CHECKED(ui_attenEnableSwitch)) {
                db -= 20;
            } else if (IS_CHECKED(ui_boostEnableSwitch)) {
                db += 20;
            }
        } else {
            // db = codec_set_mic_gain(input_gain_slider_val);
            if (IS_CHECKED(ui_ALCswitch)) {
                db = codec_set_alc_max(input_gain_slider_val);
            } else {
                db = codec_set_mic_gain(input_gain_slider_val);
            }
        }
        sprintf(s, "%.1f",db);
        lv_label_set_text(ui_mixer_db_val,s);

        lv_slider_set_value(ui_mixer_slider, input_gain_slider_val, LV_ANIM_OFF);
    }

    if (line_out_arc_val != lv_arc_get_value(ui_lineLevelArc)  || need_codec_upd) {

        db = codec_set_line_out_vol(line_out_arc_val);
        sprintf(s, "%.1f",db);
        lv_label_set_text(ui_line_level_value,s);
        lv_arc_set_value(ui_lineLevelArc, line_out_arc_val);

    }

    if (monitor_arc_val != lv_arc_get_value(ui_ouputLevelArc) || need_codec_upd) {

        if (IS_CHECKED(ui_hp_spk_switch)) {
            db = codec_set_speaker_vol(monitor_arc_val);
        } else {
            db = codec_set_hp_vol(monitor_arc_val);
        }
        sprintf(s, "%.1f",db);
        lv_label_set_text(ui_mixer_arc_value, s);
        lv_arc_set_value(ui_ouputLevelArc, monitor_arc_val);

    }

    if (checkEscapeButtonEvent(button_event)) {
        if (cur_obj != NULL) {
            UNFOCUS(cur_obj);
            UNPRESS(cur_obj);
        }
    }
    if (need_codec_upd) xTaskNotify(wav_task_handle,MONITOR_START,eSetValueWithOverwrite);
    need_codec_upd = 0;
    // to do : update vu bar from codec data
    //
    //
}


extern QueueHandle_t freq_queue;

void ui_updateWaveGenScreen (int encoder_delta, button_t* button_event){
    static lv_obj_t * cur_obj = NULL;
    static int i_obj_focused = 0;
    const int n_objects  = 2;
    bool need_codec_upd = first_scr_upd;
    first_scr_upd = false;

    if (encoder_delta) {
        bool menu_scroll = false;
        if (cur_obj != NULL) {

            if (IS_PRESSED(cur_obj)) {
                if (cur_obj == ui_mixer_slider) {
                    int cur_val = lv_slider_get_value(ui_mixer_slider);
                    wave_gen_slider_val = encoderCalculatePosition(encoder_delta, cur_val, 0, 100);
                } else if (cur_obj == ui_lineLevelArc) {
                    int cur_val = lv_arc_get_value(ui_lineLevelArc);
                    wave_gen_freq_arc_val = encoderCalculatePosition(encoder_delta, cur_val, 0, 100);
                }

            } else {
                UNFOCUS(cur_obj);
                menu_scroll = true;
            }
        }

        if (menu_scroll || cur_obj==NULL) {
            i_obj_focused = encoderCalculatePosition(encoder_delta, i_obj_focused, 0, n_objects);
            switch (i_obj_focused) {

                case 0:
                    cur_obj = NULL;
                    break;
                case 1:
                    cur_obj = ui_mixer_slider;
                    break;
                case 2:
                    cur_obj = ui_lineLevelArc;
                    break;
                default:
                    break;

            }

            if (cur_obj != NULL) {
                FOCUS(cur_obj);
            }
        }


    }

    checkEscapeButtonEvent(button_event);

    if (button_event != NULL && cur_obj != NULL) {
        if (button_event->pin == BTN_ENC_PIN) {
            PRESS(cur_obj);
        } else if (button_event->pin == BTN_LEFT_PIN) {
            UNPRESS(cur_obj);
        }
    }

    char s [10] = "";

    if (wave_gen_slider_val != lv_slider_get_value(ui_mixer_slider) || need_codec_upd) {
        float db = codec_set_line_out_vol(wave_gen_slider_val);
        sprintf(s, "%.1f",db);
        lv_label_set_text(ui_mixer_db_val,s);
        lv_slider_set_value(ui_mixer_slider, wave_gen_slider_val, LV_ANIM_OFF);
    }

    if (wave_gen_freq_arc_val != lv_arc_get_value(ui_lineLevelArc) || need_codec_upd) {
        static const uint16_t freq_table [101] = {
                30,40,50,60,70,80,90,100,110,120,130,
                140,150,160,170,
                180,190,200,210,220,230,240,250,260,
                280,300,320,340,360,380,400,420,440,
                460,480,500,520,540,560,580,600,620,640,
                680, 700,720,760,800,820,
                880,900,940,1000,1050,1100,1150,1200,
                1250,1300,1350,1400,1450,
                1500,1600,1700,1800,1900,2000,2200,2400,2600,2800,
                3000,3500,4000,4500,5000,5500,6000,6500,7000,7500,
                8000,8500,9000,9500,10000,10500,11000,11500,12000,12500,
                13000,14000,15000,16000,17000,18000,19000,20000,22000
        };

        lv_arc_set_value(ui_lineLevelArc, wave_gen_freq_arc_val);
        uint16_t freq = freq_table[wave_gen_freq_arc_val];
        sprintf(s, "%d",freq);
        lv_label_set_text(ui_line_level_value,s);
        xQueueSend(freq_queue, &freq, 20);
        xTaskNotify(wav_task_handle, WAVEGEN_START,eSetValueWithOverwrite);
    }
    need_codec_upd = 0;
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

    if (checkEscapeButtonEvent(button_event)) { /*go to menu*/}
}

void ui_updateRecScreen (int encoder_delta, button_t* button_event){
    static bool rec_started = 0;
    static bool paused = 0;

    if (button_event != NULL) {
        if (button_event->pin == BTN_ENC_PIN) {
            if (!rec_started) {
                // start recording
                //
                if (wav_start_record()) {
                    xTaskNotify(wav_task_handle, RECORD_START, eSetValueWithOverwrite);
                    rec_started = true;
                    lv_img_set_src(ui_rec_play_right_button, &ui_img_pause_png);
                    lv_img_set_src(ui_rec_play_left_button, &ui_img_stop_png);
                } else {
                    lv_label_set_text(ui_elapsed_time_label, "ERROR");
                }

            }else if (rec_started && !paused) {
                paused = true;
                lv_img_set_src(ui_rec_play_right_button, &ui_img_rec_png);
                lv_img_set_src(ui_rec_play_left_button, &ui_img_mixer_png);
                //pause record
                //
                xTaskNotify(wav_task_handle,AUDIO_STOP,eSetValueWithOverwrite);
            } else if (rec_started && paused) {
                paused = false;
                lv_img_set_src(ui_rec_play_right_button, &ui_img_pause_png);
                lv_img_set_src(ui_rec_play_left_button, &ui_img_stop_png);
                // resume record
                //
                xTaskNotify(wav_task_handle,RECORD_START,eSetValueWithOverwrite);
            }
        }

        if (button_event->pin == BTN_LEFT_PIN) {
            if (lv_img_get_src(ui_rec_play_left_button) == &ui_img_mixer_png) {
                openMixerMenu();
            } else {
                xTaskNotify(wav_task_handle,AUDIO_STOP,eSetValueWithOverwrite);
                vTaskDelay(pdMS_TO_TICKS(50));
                openAcceptDeclineMenu();
            }
        }

        if (checkEscapeButtonEvent(button_event)) {
            // mute codec
            //
            xTaskNotify(wav_task_handle,AUDIO_STOP,eSetValueWithOverwrite);
            vTaskDelay(pdMS_TO_TICKS(50));
            openAcceptDeclineMenu();
        }
    }

}

void ui_updatePlayScreen (int encoder_delta, button_t* button_event) {

    static bool paused = 0;

    if (button_event != NULL) {

        if (checkEscapeButtonEvent(button_event) || file_error) {
            started = false;
            // stop audio
            xTaskNotify(wav_task_handle,AUDIO_STOP,eSetValueWithOverwrite);
            return;
        }
        if (button_event->pin == BTN_ENC_PIN) {
            if (!started) {
                started = true;
                lv_img_set_src(ui_rec_play_right_button, &ui_img_pause_png);
                // start playing
                //
                char fname [255] = "";
                get_nth_file_name(file_selected_pos,fname);
                wav_open_file(fname);
                xTaskNotify(wav_task_handle,PLAYER_PLAY,eSetValueWithOverwrite);
            }else if (started && !paused) {
                paused = true;
                lv_img_set_src(ui_rec_play_right_button, &ui_img_play_png);
                // pause
                //
                xTaskNotify(wav_task_handle,AUDIO_STOP,eSetValueWithOverwrite);
            } else if (started && paused) {
                paused = false;
                xTaskNotify(wav_task_handle,PLAYER_PLAY,eSetValueWithOverwrite);
                lv_img_set_src(ui_rec_play_right_button, &ui_img_pause_png);
            }
        }

        if (button_event->pin == BTN_LEFT_PIN) {
            openPlayerMixerMenu();
        }
    }

    uint32_t val = 0;
    if ( xTaskNotifyWait(0,ULONG_MAX,&val, 10) == pdTRUE) {
        increment_elapsed_time(1);
    }

}


void ui_updatePlayerMixer (int encoder_delta, button_t* button_event) {

    static lv_obj_t * cur_obj = NULL;
    static int i_obj_focused = 0;
    const int n_objects  = 2;
    bool need_codec_upd = first_scr_upd;
    first_scr_upd = false;

    if (encoder_delta) {

        bool menu_scroll = false;
        if (cur_obj != NULL) {

            if (IS_PRESSED(cur_obj)) {
                if (cur_obj == ui_lineLevelArc) {
                    int cur_val = lv_arc_get_value(ui_lineLevelArc);
                    line_out_arc_val = encoderCalculatePosition(encoder_delta, cur_val, 0, 100);
                } else if (cur_obj == ui_ouputLevelArc){
                    int cur_val = lv_arc_get_value(ui_ouputLevelArc);
                    monitor_arc_val = encoderCalculatePosition(encoder_delta, cur_val, 0, 100);
                }

            }else {
                UNFOCUS(cur_obj);
                if (cur_obj == ui_ouputLevelArc) UNFOCUS(ui_lineLevelArc);
                menu_scroll = true;
            }
        }

        if (menu_scroll || cur_obj==NULL) {
            i_obj_focused = encoderCalculatePosition(encoder_delta, i_obj_focused, 0, n_objects);
            switch (i_obj_focused) {

                case 0:
                    cur_obj = NULL;
                    break;
                case 1:
                    cur_obj = ui_lineLevelArc;
                    break;
                case 2:
                    cur_obj = ui_hp_spk_switch;
                    break;
                default:
                    break;

            }

            if (cur_obj != NULL) {
                FOCUS(cur_obj);
            }
        }


    }

    if (button_event != NULL) {

        if (button_event->pin == BTN_LEFT_PIN &&
            lv_disp_get_scr_prev(NULL) == ui_recPlayScreen) { // left btn => esc to rec menu
            if (cur_obj != NULL) {
                if (!IS_PRESSED(cur_obj)) {
                    openPlayMenu();
                    return;
                }
            } else {
                openPlayMenu();
                return;
            }
        }

        if (cur_obj != NULL) {
            if (cur_obj == ui_lineLevelArc) {

                if (button_event->pin == BTN_ENC_PIN) {
                    if (IS_PRESSED(ui_lineLevelArc)) {
                        UNPRESS(ui_lineLevelArc);
                        PRESS (ui_ouputLevelArc);
                        cur_obj = ui_ouputLevelArc;
                    } else {
                        PRESS(ui_lineLevelArc);
                    }
                } else if (button_event->pin == BTN_LEFT_PIN) {
                    UNPRESS(ui_lineLevelArc);
                }

            } else if (cur_obj == ui_ouputLevelArc) {
                if (button_event->pin == BTN_ENC_PIN) {
                    if (IS_PRESSED(ui_ouputLevelArc)) {
                        UNPRESS(ui_ouputLevelArc);
                        PRESS (ui_lineLevelArc);
                        cur_obj = ui_lineLevelArc;
                    } else {
                        PRESS(ui_ouputLevelArc);
                    }
                } else if (button_event->pin == BTN_LEFT_PIN) {
                    UNPRESS(ui_ouputLevelArc);
                }
            }  else if (cur_obj == ui_hp_spk_switch) {
                if (button_event->pin == BTN_ENC_PIN) {
                    need_codec_upd = true;
                    if (IS_CHECKED(ui_hp_spk_switch)) {
                        UNCHECK_OBJ(ui_hp_spk_switch);
                        codec_set_speaker_vol(0);
                    } else {
                        CHECK_OBJ(ui_hp_spk_switch);
                        codec_set_hp_vol(0);
                    }
                }
            }
        }
    }

    char s [10] = "";
    float db = 0;

    if (line_out_arc_val != lv_arc_get_value(ui_lineLevelArc) || need_codec_upd){
        db = codec_set_line_out_vol(line_out_arc_val);
        sprintf(s, "%.1f",db);
        lv_label_set_text(ui_line_level_value,s);
        lv_arc_set_value(ui_lineLevelArc, line_out_arc_val);


    }

    if (monitor_arc_val != lv_arc_get_value(ui_ouputLevelArc) || need_codec_upd) {
        if (IS_CHECKED(ui_hp_spk_switch)) {
            db = codec_set_speaker_vol(monitor_arc_val);
        } else {
            db = codec_set_hp_vol(monitor_arc_val);
        }
        sprintf(s, "%.1f",db);
        lv_label_set_text(ui_mixer_arc_value, s);
        lv_arc_set_value(ui_ouputLevelArc, monitor_arc_val);
    }

    if (checkEscapeButtonEvent(button_event)) {
        if (cur_obj != NULL) {
            UNFOCUS(cur_obj);
            UNPRESS(cur_obj);
        }
    }


    need_codec_upd = 0;
    // update vu bar from wav data
    //
    uint32_t val = 0;
    if ( xTaskNotifyWait(0,ULONG_MAX,&val, 10) == pdTRUE) {
        val -= 100-lv_slider_get_value(ui_mixer_slider);
        lv_slider_set_value(ui_mixer_vu_bar_slider, val, LV_ANIM_OFF);
        increment_elapsed_time(1);
    }

}

void ui_updateAcceptDeclineScreen (int encoder_delta, button_t* button_event){

    if (button_event != NULL) {
        if (button_event->pin == BTN_LEFT_PIN) {
            wav_delete_record();
        } else if (button_event->pin == BTN_ENC_PIN) {
            wav_save_record();

            file_selected_pos = 0;
            file_selected_pos_prev = -1;
        }

        changeScreen(ui_startScreen);
        currentScreenUpd = ui_updateStartScreen;
    }

}

void ui_updateOptionsScreen (int encoder_delta, button_t* button_event){
    static lv_obj_t * cur_obj = NULL;
    static int i_obj_focused = 0;
    const int n_objects  = 6;

    if (encoder_delta) {
        if (cur_obj != NULL) {
            if (cur_obj == ui_brightness_slider && IS_PRESSED(cur_obj)) {
                incrementObj(SLIDER, ui_brightness_slider, encoder_delta);
                set_pwm_backlight (lv_slider_get_value(ui_brightness_slider));
                return;
            }
        }

        i_obj_focused = encoderCalculatePosition(encoder_delta, i_obj_focused, 0, n_objects);
        switch (i_obj_focused) {
            case 0:
                cur_obj = NULL;
                break;
            case 1:
                cur_obj = ui_phantomSwitch;
                break;
            case 2:
                cur_obj = ui_ALCswitch;
                break;
            case 3:
                cur_obj = ui_attenEnableSwitch;
                break;
            case 4:
                cur_obj = ui_xlr_swap_switch;
                break;
            case 5:
                cur_obj = ui_boostEnableSwitch;
                break;
            case 6:
                cur_obj = ui_brightness_slider;
                break;
            default:
                break;
        }
        if (cur_obj != NULL) {
            FOCUS(cur_obj);
        }
    }

    if (checkEscapeButtonEvent(button_event)) {}

    if (button_event != NULL) {
        if (cur_obj == ui_brightness_slider) {
            if (button_event->pin == BTN_ENC_PIN) {
                PRESS(ui_brightness_slider);
            } else if (button_event->pin == BTN_LEFT_PIN) {
                UNPRESS(ui_brightness_slider);
            }
            return;
        }
        if (IS_CHECKED(cur_obj)) {
            UNCHECK_OBJ(cur_obj);
        } else {
            CHECK_OBJ(cur_obj);
        }

        bool state = IS_CHECKED(cur_obj);

        switch (i_obj_focused) {
            case 0:
                // NULL;
                break;
            case 1:
                //ui_phantomSwitch;
                enable_phantom(state);
                break;
            case 2:
                //ui_ALCswitch;
                codec_enable_alc(state);
                break;
            case 3:
                //ui_attenEnableSwitch;
                enable_atten(state);
                break;
            case 4:
                //ui_xlr_swap_switch;
                enable_xlr_swap(state);
                break;
            case 5:
                //ui_boostEnableSwitch;
                codec_enable_line_boost(state);
                break;
            case 6:
                //ui_brightness_slider;
                break;
            default:
                break;
        }
    }
}

void ui_updateCableTestScreen (int encoder_delta, button_t* button_event) {

    if (checkEscapeButtonEvent(button_event)) {}

}
