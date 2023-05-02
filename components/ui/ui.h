// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.1
// LVGL VERSION: 8.2.0
// PROJECT: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

extern lv_obj_t * ui_startScreen;
extern lv_obj_t * ui_startMenu;
extern lv_obj_t * ui_start_menu_roller;
extern lv_obj_t * ui_battery_bar;
extern lv_obj_t * ui_battery_val;
extern lv_obj_t * ui_mixerMenuScreen;
extern lv_obj_t * ui_levelMenu;
extern lv_obj_t * ui_mixer_vu_bar_slider;
extern lv_obj_t * ui_mixer_slider;
extern lv_obj_t * ui_mixer_db_val;
extern lv_obj_t * ui_lineLevelArc;
extern lv_obj_t * ui_ouputLevelArc;
extern lv_obj_t * ui_mixer_arc_value;
extern lv_obj_t * ui_line_out_label;
extern lv_obj_t * ui_line_level_value;
extern lv_obj_t * ui_mixer_arc_label;
extern lv_obj_t * ui_spkIcon;
extern lv_obj_t * ui_hpIcon;
extern lv_obj_t * ui_mic_icon;
extern lv_obj_t * ui_xlr_icon;
extern lv_obj_t * ui_mic_line_switch;
extern lv_obj_t * ui_hp_spk_switch;
extern lv_obj_t * ui_mixer_slider_func_label;
extern lv_obj_t * ui_recPlayScreen;
extern lv_obj_t * ui_recPlayMenu;
extern lv_obj_t * ui_chart;
extern lv_obj_t * ui_elapsed_time_label;
extern lv_obj_t * ui_timePosSlider;
extern lv_obj_t * ui_rec_play_left_button;
extern lv_obj_t * ui_rec_play_right_button;
extern lv_obj_t * ui_rec_play_track_len_label;
extern lv_obj_t * ui_tracklistScreen;
extern lv_obj_t * ui_tracklistMenu;
extern lv_obj_t * ui_trackListRoller;
extern lv_obj_t * ui_acceptDeclineScreen;
extern lv_obj_t * ui_choiceMenu;
extern lv_obj_t * ui_buttonDecline;
extern lv_obj_t * ui_declineLabel;
extern lv_obj_t * ui_buttonAccept;
extern lv_obj_t * ui_acceptLabel;
extern lv_obj_t * ui_saveLabel;
extern lv_obj_t * ui_optionsScreen;
extern lv_obj_t * ui_settingsMenu;
extern lv_obj_t * ui_phantomSwitch;

extern lv_obj_t * ui_attenEnableSwitch;
extern lv_obj_t * ui_labelPhantom;

extern lv_obj_t * ui_attenEnableLabel;
extern lv_obj_t * ui_xlr_swap_switch;
extern lv_obj_t * ui_boostEnableSwitch;
extern lv_obj_t * ui_brightness_slider;
extern lv_obj_t * ui_brightness_label;
extern lv_obj_t * ui_Labelxlrswap;
extern lv_obj_t * ui_optionsScreen_Label5;
extern lv_obj_t * ui_boostEnableLabel;
extern lv_obj_t * ui_cableTestScreen;
extern lv_obj_t * ui_cableTesterMenu;
extern lv_obj_t * ui_OK_icon;
extern lv_obj_t * ui_cableStatus;
extern lv_obj_t * ui_cableTestScreen_Label3;


LV_IMG_DECLARE(ui_img_indicator_ver_png);    // assets/indicator_ver.png
LV_IMG_DECLARE(ui_img_pot_ver_knob_png);    // assets/pot_ver_knob.png
LV_IMG_DECLARE(ui_img_speaker_png);    // assets/speaker.png
LV_IMG_DECLARE(ui_img_headphone_png);    // assets/headphone.png
LV_IMG_DECLARE(ui_img_mic_png);    // assets/mic.png
LV_IMG_DECLARE(ui_img_xlr_png);    // assets/xlr.png
LV_IMG_DECLARE(ui_img_mixer_png);    // assets/mixer.png
LV_IMG_DECLARE(ui_img_pause_png);    // assets/pause.png
LV_IMG_DECLARE(ui_img_accept_png);    // assets/accept.png
LV_IMG_DECLARE(ui_img_rec_png);
LV_IMG_DECLARE(ui_img_stop_png);
LV_IMG_DECLARE(ui_img_play_png);



void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
