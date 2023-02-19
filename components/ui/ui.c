// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.1
// LVGL VERSION: 8.2.0
// PROJECT: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"
#include "ui_comp.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_startScreen;
lv_obj_t * ui_startMenu;
lv_obj_t * ui_modeRoller;
lv_obj_t * ui_batteryBar;
lv_obj_t * ui_Screen1_Label1;
lv_obj_t * ui_mixerMenuScreen;
lv_obj_t * ui_levelMenu;
lv_obj_t * ui_vuSlider;
lv_obj_t * ui_inputSlider;
lv_obj_t * ui_dbLabel;
lv_obj_t * ui_ouputLevelArc;
lv_obj_t * ui_outLevelLabel;
lv_obj_t * ui_spkIcon;
lv_obj_t * ui_hpIcon;
lv_obj_t * ui_Screen1_Image3;
lv_obj_t * ui_Screen1_Image4;
lv_obj_t * ui_hpSpkSwitch;
lv_obj_t * ui_hpSpkSwitch1;
lv_obj_t * ui_recPlayScreen;
lv_obj_t * ui_recPlayMenu;
lv_obj_t * ui_chart;
lv_obj_t * ui_elapsedTimeLabel;
lv_obj_t * ui_timePosSlider;
lv_obj_t * ui_playMenuBtnLeft;
lv_obj_t * ui_playMenuBtnRight;
lv_obj_t * ui_tracklistScreen;
lv_obj_t * ui_tracklistMenu;
lv_obj_t * ui_trackListRoller;
lv_obj_t * ui_acceptDeclineScreen;
lv_obj_t * ui_choiceMenu;
lv_obj_t * ui_buttonDecline;
lv_obj_t * ui_declineLabel;
lv_obj_t * ui_buttonDecline1;
lv_obj_t * ui_declineLabel1;
lv_obj_t * ui_saveLabel;
lv_obj_t * ui_optionsScreen;
lv_obj_t * ui_settingsMenu;
lv_obj_t * ui_phantomSwitch;
lv_obj_t * ui_label48v;
lv_obj_t * ui_spkEnableSwitch;
lv_obj_t * ui_beepEnableSwitch;
lv_obj_t * ui_labelPhantom;
lv_obj_t * ui_spkEnableLabel;
lv_obj_t * ui_beepEnableLabel;
lv_obj_t * ui_cableTestScreen;
lv_obj_t * ui_cableTesterMenu;
lv_obj_t * ui_acceptIcon;
lv_obj_t * ui_noCableLabel;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
   // #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_startScreen_screen_init(void)
{
    ui_startScreen = lv_obj_create(NULL);

    ui_startMenu = lv_obj_create(ui_startScreen);
    lv_obj_set_width(ui_startMenu, 240);
    lv_obj_set_height(ui_startMenu, 240);
    lv_obj_set_align(ui_startMenu, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_startMenu, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_modeRoller = lv_roller_create(ui_startMenu);
    lv_roller_set_options(ui_modeRoller, "Monitor\nPlay\nRecord\nTest signal\nCable tester\nOptions",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_modeRoller, 240);
    lv_obj_set_height(ui_modeRoller, 240);
    lv_obj_set_align(ui_modeRoller, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(ui_modeRoller, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_modeRoller, lv_color_hex(0x696464), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_modeRoller, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_modeRoller, lv_color_hex(0x999292), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_modeRoller, 0, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_modeRoller, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_modeRoller, LV_GRAD_DIR_VER, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_batteryBar = lv_bar_create(ui_startMenu);
    lv_bar_set_value(ui_batteryBar, 67, LV_ANIM_OFF);
    lv_obj_set_width(ui_batteryBar, 57);
    lv_obj_set_height(ui_batteryBar, 24);
    lv_obj_set_x(ui_batteryBar, 75);
    lv_obj_set_y(ui_batteryBar, -96);
    lv_obj_set_align(ui_batteryBar, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_batteryBar, LV_STATE_PRESSED);       /// States
    lv_obj_set_style_radius(ui_batteryBar, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_batteryBar, LV_BLEND_MODE_SUBTRACTIVE, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(ui_batteryBar, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_batteryBar, lv_color_hex(0xD75B5D), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_batteryBar, 145, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_batteryBar, lv_color_hex(0xD75BD5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_batteryBar, LV_GRAD_DIR_HOR, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_batteryBar, lv_color_hex(0x3C9C0F), LV_PART_INDICATOR | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_batteryBar, 150, LV_PART_INDICATOR | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(ui_batteryBar, lv_color_hex(0xA0D453), LV_PART_INDICATOR | LV_STATE_PRESSED);

    ui_Screen1_Label1 = lv_label_create(ui_batteryBar);
    lv_obj_set_width(ui_Screen1_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Screen1_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Screen1_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Screen1_Label1, "67%");
    lv_obj_set_style_text_color(ui_Screen1_Label1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Screen1_Label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

}
void ui_mixerMenuScreen_screen_init(void)
{
    ui_mixerMenuScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_mixerMenuScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_levelMenu = lv_obj_create(ui_mixerMenuScreen);
    lv_obj_set_width(ui_levelMenu, 240);
    lv_obj_set_height(ui_levelMenu, 240);
    lv_obj_set_align(ui_levelMenu, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_levelMenu, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_scrollbar_mode(ui_levelMenu, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_scroll_dir(ui_levelMenu, LV_DIR_HOR);

    ui_vuSlider = lv_slider_create(ui_levelMenu);
    lv_slider_set_value(ui_vuSlider, 100, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_vuSlider) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_vuSlider, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_vuSlider, 29);
    lv_obj_set_height(ui_vuSlider, 176);
    lv_obj_set_x(ui_vuSlider, -43);
    lv_obj_set_y(ui_vuSlider, 17);
    lv_obj_set_align(ui_vuSlider, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_vuSlider, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                      LV_OBJ_FLAG_SNAPPABLE);     /// Flags
    lv_obj_set_style_radius(ui_vuSlider, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_vuSlider, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_vuSlider, 23, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(ui_vuSlider, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_vuSlider, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_vuSlider, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_vuSlider, &ui_img_indicator_ver_png, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_tiled(ui_vuSlider, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_vuSlider, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_vuSlider, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_inputSlider = lv_slider_create(ui_levelMenu);
    lv_slider_set_value(ui_inputSlider, 60, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_inputSlider) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_inputSlider, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_inputSlider, 8);
    lv_obj_set_height(ui_inputSlider, 210);
    lv_obj_set_x(ui_inputSlider, -86);
    lv_obj_set_y(ui_inputSlider, 1);
    lv_obj_set_align(ui_inputSlider, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_inputSlider, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_inputSlider, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_inputSlider, 34, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_tiled(ui_inputSlider, false, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_inputSlider, lv_color_hex(0xF6F6F6), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_opa(ui_inputSlider, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui_inputSlider, 45, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_spread(ui_inputSlider, 3, LV_PART_MAIN | LV_STATE_CHECKED);

    lv_obj_set_style_radius(ui_inputSlider, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_inputSlider, lv_color_hex(0xFDFDFD), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_inputSlider, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_inputSlider, lv_color_hex(0xFFA62B), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_inputSlider, 56, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_inputSlider, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_inputSlider, LV_GRAD_DIR_VER, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_inputSlider, LV_BLEND_MODE_ADDITIVE, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_inputSlider, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_inputSlider, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_inputSlider, &ui_img_pot_ver_knob_png, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_tiled(ui_inputSlider, false, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_inputSlider, 0, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_bg_img_src(ui_inputSlider, &ui_img_pot_ver_knob_png, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_bg_img_recolor(ui_inputSlider, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_bg_img_recolor_opa(ui_inputSlider, 123, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_color(ui_inputSlider, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui_inputSlider, 255, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui_inputSlider, 45, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui_inputSlider, 4, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_x(ui_inputSlider, 0, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_y(ui_inputSlider, 2, LV_PART_KNOB | LV_STATE_PRESSED);

    ui_dbLabel = lv_label_create(ui_levelMenu);
    lv_obj_set_width(ui_dbLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_dbLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_dbLabel, -47);
    lv_obj_set_y(ui_dbLabel, -95);
    lv_obj_set_align(ui_dbLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_dbLabel, "-12.6");
    lv_obj_set_style_text_font(ui_dbLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ouputLevelArc = lv_arc_create(ui_levelMenu);
    lv_obj_set_width(ui_ouputLevelArc, 110);
    lv_obj_set_height(ui_ouputLevelArc, 111);
    lv_obj_set_x(ui_ouputLevelArc, 49);
    lv_obj_set_y(ui_ouputLevelArc, 6);
    lv_obj_set_align(ui_ouputLevelArc, LV_ALIGN_CENTER);
    lv_arc_set_value(ui_ouputLevelArc, 40);
    lv_obj_set_style_arc_color(ui_ouputLevelArc, lv_color_hex(0x9CA6FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_ouputLevelArc, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_ouputLevelArc, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_ouputLevelArc, 67, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_color(ui_ouputLevelArc, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_opa(ui_ouputLevelArc, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui_ouputLevelArc, 56, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_spread(ui_ouputLevelArc, 2, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_blend_mode(ui_ouputLevelArc, LV_BLEND_MODE_ADDITIVE, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(ui_ouputLevelArc, 60, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ui_ouputLevelArc, lv_color_hex(0x3F3F3F), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_ouputLevelArc, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_color(ui_ouputLevelArc, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui_ouputLevelArc, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui_ouputLevelArc, 45, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui_ouputLevelArc, 4, LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_set_style_arc_color(ui_ouputLevelArc, lv_color_hex(0xEE3B0F), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_ouputLevelArc, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_ouputLevelArc, 7, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_ouputLevelArc, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ouputLevelArc, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_ouputLevelArc, lv_color_hex(0xDDB8F7), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_ouputLevelArc, 30, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_ouputLevelArc, 200, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_ouputLevelArc, LV_GRAD_DIR_HOR, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_outLevelLabel = lv_label_create(ui_ouputLevelArc);
    lv_obj_set_width(ui_outLevelLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_outLevelLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_outLevelLabel, -2);
    lv_obj_set_y(ui_outLevelLabel, -1);
    lv_obj_set_align(ui_outLevelLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_outLevelLabel, "-10.3");
    lv_obj_set_style_text_font(ui_outLevelLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_spkIcon = lv_img_create(ui_levelMenu);
    lv_img_set_src(ui_spkIcon, &ui_img_speaker_png);
    lv_obj_set_width(ui_spkIcon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_spkIcon, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_spkIcon, 95);
    lv_obj_set_y(ui_spkIcon, 95);
    lv_obj_set_align(ui_spkIcon, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_spkIcon, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_spkIcon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_hpIcon = lv_img_create(ui_levelMenu);
    lv_img_set_src(ui_hpIcon, &ui_img_headphone_png);
    lv_obj_set_width(ui_hpIcon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_hpIcon, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_hpIcon, 5);
    lv_obj_set_y(ui_hpIcon, 95);
    lv_obj_set_align(ui_hpIcon, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_hpIcon, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_hpIcon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen1_Image3 = lv_img_create(ui_levelMenu);
    lv_img_set_src(ui_Screen1_Image3, &ui_img_mic_png);
    lv_obj_set_width(ui_Screen1_Image3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Screen1_Image3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Screen1_Image3, 5);
    lv_obj_set_y(ui_Screen1_Image3, -94);
    lv_obj_set_align(ui_Screen1_Image3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Screen1_Image3, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Screen1_Image3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen1_Image4 = lv_img_create(ui_levelMenu);
    lv_img_set_src(ui_Screen1_Image4, &ui_img_xlr_png);
    lv_obj_set_width(ui_Screen1_Image4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Screen1_Image4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Screen1_Image4, 95);
    lv_obj_set_y(ui_Screen1_Image4, -95);
    lv_obj_set_align(ui_Screen1_Image4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Screen1_Image4, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Screen1_Image4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_hpSpkSwitch = lv_switch_create(ui_levelMenu);
    lv_obj_set_width(ui_hpSpkSwitch, 42);
    lv_obj_set_height(ui_hpSpkSwitch, 18);
    lv_obj_set_x(ui_hpSpkSwitch, 49);
    lv_obj_set_y(ui_hpSpkSwitch, -95);
    lv_obj_set_align(ui_hpSpkSwitch, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_hpSpkSwitch, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_hpSpkSwitch, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_hpSpkSwitch, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_hpSpkSwitch, lv_color_hex(0x525252), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_hpSpkSwitch, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_hpSpkSwitch, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_hpSpkSwitch, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_hpSpkSwitch, lv_color_hex(0xFFF7F6), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui_hpSpkSwitch, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui_hpSpkSwitch, 30, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui_hpSpkSwitch, 0, LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_set_style_radius(ui_hpSpkSwitch, 8, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui_hpSpkSwitch, lv_color_hex(0xFF2C0A), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_hpSpkSwitch, 150, LV_PART_INDICATOR | LV_STATE_CHECKED);

    lv_obj_set_style_radius(ui_hpSpkSwitch, 8, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_hpSpkSwitch1 = lv_switch_create(ui_levelMenu);
    lv_obj_set_width(ui_hpSpkSwitch1, 42);
    lv_obj_set_height(ui_hpSpkSwitch1, 18);
    lv_obj_set_x(ui_hpSpkSwitch1, 49);
    lv_obj_set_y(ui_hpSpkSwitch1, 96);
    lv_obj_set_align(ui_hpSpkSwitch1, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_hpSpkSwitch1, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_hpSpkSwitch1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_hpSpkSwitch1, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_hpSpkSwitch1, lv_color_hex(0x525252), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_hpSpkSwitch1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_hpSpkSwitch1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_hpSpkSwitch1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_hpSpkSwitch1, lv_color_hex(0xFFF7F6), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui_hpSpkSwitch1, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui_hpSpkSwitch1, 30, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui_hpSpkSwitch1, 0, LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_set_style_radius(ui_hpSpkSwitch1, 8, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui_hpSpkSwitch1, lv_color_hex(0xFF2C0A), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_hpSpkSwitch1, 150, LV_PART_INDICATOR | LV_STATE_CHECKED);

    lv_obj_set_style_radius(ui_hpSpkSwitch1, 8, LV_PART_KNOB | LV_STATE_DEFAULT);

}
void ui_recPlayScreen_screen_init(void)
{
    ui_recPlayScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_recPlayScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_recPlayMenu = lv_obj_create(ui_recPlayScreen);
    lv_obj_set_width(ui_recPlayMenu, 240);
    lv_obj_set_height(ui_recPlayMenu, 240);
    lv_obj_set_x(ui_recPlayMenu, -1);
    lv_obj_set_y(ui_recPlayMenu, -2);
    lv_obj_set_align(ui_recPlayMenu, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_recPlayMenu, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_chart = lv_chart_create(ui_recPlayMenu);
    lv_obj_set_width(ui_chart, 223);
    lv_obj_set_height(ui_chart, 104);
    lv_obj_set_x(ui_chart, 1);
    lv_obj_set_y(ui_chart, -58);
    lv_obj_set_align(ui_chart, LV_ALIGN_CENTER);

    ui_elapsedTimeLabel = lv_label_create(ui_recPlayMenu);
    lv_obj_set_width(ui_elapsedTimeLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_elapsedTimeLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_elapsedTimeLabel, -4);
    lv_obj_set_y(ui_elapsedTimeLabel, 54);
    lv_obj_set_align(ui_elapsedTimeLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_elapsedTimeLabel, "0:34");
    lv_obj_set_style_text_font(ui_elapsedTimeLabel, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_timePosSlider = lv_slider_create(ui_recPlayMenu);
    lv_slider_set_value(ui_timePosSlider, 40, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_timePosSlider) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_timePosSlider, 0,
                                                                                                  LV_ANIM_OFF);
    lv_obj_set_width(ui_timePosSlider, 209);
    lv_obj_set_height(ui_timePosSlider, 6);
    lv_obj_set_x(ui_timePosSlider, 0);
    lv_obj_set_y(ui_timePosSlider, 8);
    lv_obj_set_align(ui_timePosSlider, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_timePosSlider, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_timePosSlider, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_timePosSlider, 34, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_timePosSlider, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_timePosSlider, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(ui_timePosSlider, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_timePosSlider, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_timePosSlider, 255, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_playMenuBtnLeft = lv_imgbtn_create(ui_recPlayMenu);
    lv_imgbtn_set_src(ui_playMenuBtnLeft, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_mixer_png, NULL);
    lv_imgbtn_set_src(ui_playMenuBtnLeft, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_stop_png, NULL);
    lv_obj_set_height(ui_playMenuBtnLeft, 52);
    lv_obj_set_width(ui_playMenuBtnLeft, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_playMenuBtnLeft, -79);
    lv_obj_set_y(ui_playMenuBtnLeft, 81);
    lv_obj_set_align(ui_playMenuBtnLeft, LV_ALIGN_CENTER);

    ui_playMenuBtnRight = lv_imgbtn_create(ui_recPlayMenu);
    lv_imgbtn_set_src(ui_playMenuBtnRight, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_rec_png, NULL);
    lv_imgbtn_set_src(ui_playMenuBtnRight, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_pause_png, NULL);
    lv_imgbtn_set_src(ui_playMenuBtnRight, LV_IMGBTN_STATE_DISABLED, NULL, &ui_img_play_png, NULL);
    lv_obj_set_height(ui_playMenuBtnRight, 51);
    lv_obj_set_width(ui_playMenuBtnRight, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_playMenuBtnRight, 79);
    lv_obj_set_y(ui_playMenuBtnRight, 78);
    lv_obj_set_align(ui_playMenuBtnRight, LV_ALIGN_CENTER);

}
void ui_tracklistScreen_screen_init(void)
{
    ui_tracklistScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_tracklistScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_tracklistMenu = lv_obj_create(ui_tracklistScreen);
    lv_obj_set_width(ui_tracklistMenu, 240);
    lv_obj_set_height(ui_tracklistMenu, 240);
    lv_obj_set_align(ui_tracklistMenu, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_tracklistMenu, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_trackListRoller = lv_roller_create(ui_tracklistMenu);
    lv_roller_set_options(ui_trackListRoller, "music - track 12  \nmetallica - nothing else\nrecord 001\nj.s bach - cello ",
                          LV_ROLLER_MODE_INFINITE);
    lv_obj_set_width(ui_trackListRoller, 240);
    lv_obj_set_height(ui_trackListRoller, 240);
    lv_obj_set_align(ui_trackListRoller, LV_ALIGN_CENTER);

    lv_obj_set_style_bg_color(ui_trackListRoller, lv_color_hex(0x706E71), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_trackListRoller, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_trackListRoller, lv_color_hex(0x565656), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_trackListRoller, 0, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_trackListRoller, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_trackListRoller, LV_GRAD_DIR_VER, LV_PART_SELECTED | LV_STATE_DEFAULT);

}
void ui_acceptDeclineScreen_screen_init(void)
{
    ui_acceptDeclineScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_acceptDeclineScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_choiceMenu = lv_obj_create(ui_acceptDeclineScreen);
    lv_obj_set_width(ui_choiceMenu, 240);
    lv_obj_set_height(ui_choiceMenu, 240);
    lv_obj_set_align(ui_choiceMenu, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_choiceMenu, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_buttonDecline = lv_btn_create(ui_choiceMenu);
    lv_obj_set_width(ui_buttonDecline, 100);
    lv_obj_set_height(ui_buttonDecline, 50);
    lv_obj_set_x(ui_buttonDecline, -59);
    lv_obj_set_y(ui_buttonDecline, 76);
    lv_obj_set_align(ui_buttonDecline, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_buttonDecline, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_buttonDecline, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_buttonDecline, lv_color_hex(0x4F4F4F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_buttonDecline, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_buttonDecline, lv_color_hex(0x890D0D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_buttonDecline, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_buttonDecline, 234, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_buttonDecline, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_buttonDecline, lv_color_hex(0xF47272), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_opa(ui_buttonDecline, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui_buttonDecline, 45, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_spread(ui_buttonDecline, 2, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_declineLabel = lv_label_create(ui_buttonDecline);
    lv_obj_set_width(ui_declineLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_declineLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_declineLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_declineLabel, "No");
    lv_obj_set_style_text_font(ui_declineLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_buttonDecline1 = lv_btn_create(ui_choiceMenu);
    lv_obj_set_width(ui_buttonDecline1, 100);
    lv_obj_set_height(ui_buttonDecline1, 50);
    lv_obj_set_x(ui_buttonDecline1, 58);
    lv_obj_set_y(ui_buttonDecline1, 75);
    lv_obj_set_align(ui_buttonDecline1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_buttonDecline1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_buttonDecline1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_buttonDecline1, lv_color_hex(0x4F4F4F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_buttonDecline1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_buttonDecline1, lv_color_hex(0x1BB01E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_buttonDecline1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_buttonDecline1, 234, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_buttonDecline1, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_buttonDecline1, lv_color_hex(0x5DCB60), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_opa(ui_buttonDecline1, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui_buttonDecline1, 45, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_spread(ui_buttonDecline1, 2, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_declineLabel1 = lv_label_create(ui_buttonDecline1);
    lv_obj_set_width(ui_declineLabel1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_declineLabel1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_declineLabel1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_declineLabel1, "Yes");
    lv_obj_set_style_text_font(ui_declineLabel1, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_saveLabel = lv_label_create(ui_choiceMenu);
    lv_obj_set_width(ui_saveLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_saveLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_saveLabel, 1);
    lv_obj_set_y(ui_saveLabel, -24);
    lv_obj_set_align(ui_saveLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_saveLabel, "Save?");
    lv_obj_set_style_text_font(ui_saveLabel, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);

}
void ui_optionsScreen_screen_init(void)
{
    ui_optionsScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_optionsScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_settingsMenu = lv_obj_create(ui_optionsScreen);
    lv_obj_set_width(ui_settingsMenu, lv_pct(100));
    lv_obj_set_height(ui_settingsMenu, lv_pct(100));
    lv_obj_set_align(ui_settingsMenu, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_settingsMenu, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_phantomSwitch = lv_switch_create(ui_settingsMenu);
    lv_obj_set_width(ui_phantomSwitch, 71);
    lv_obj_set_height(ui_phantomSwitch, 45);
    lv_obj_set_x(ui_phantomSwitch, 53);
    lv_obj_set_y(ui_phantomSwitch, -83);
    lv_obj_set_align(ui_phantomSwitch, LV_ALIGN_CENTER);
    lv_obj_set_style_shadow_color(ui_phantomSwitch, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui_phantomSwitch, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui_phantomSwitch, 33, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui_phantomSwitch, 2, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_label48v = lv_label_create(ui_settingsMenu);
    lv_obj_set_width(ui_label48v, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_label48v, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_label48v, -58);
    lv_obj_set_y(ui_label48v, -91);
    lv_obj_set_align(ui_label48v, LV_ALIGN_CENTER);
    lv_label_set_text(ui_label48v, "48V");
    lv_obj_set_style_text_font(ui_label48v, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_spkEnableSwitch = lv_switch_create(ui_settingsMenu);
    lv_obj_set_width(ui_spkEnableSwitch, 71);
    lv_obj_set_height(ui_spkEnableSwitch, 45);
    lv_obj_set_x(ui_spkEnableSwitch, 53);
    lv_obj_set_y(ui_spkEnableSwitch, -4);
    lv_obj_set_align(ui_spkEnableSwitch, LV_ALIGN_CENTER);
    lv_obj_set_style_shadow_color(ui_spkEnableSwitch, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui_spkEnableSwitch, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui_spkEnableSwitch, 33, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui_spkEnableSwitch, 2, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_beepEnableSwitch = lv_switch_create(ui_settingsMenu);
    lv_obj_set_width(ui_beepEnableSwitch, 71);
    lv_obj_set_height(ui_beepEnableSwitch, 45);
    lv_obj_set_x(ui_beepEnableSwitch, 53);
    lv_obj_set_y(ui_beepEnableSwitch, 78);
    lv_obj_set_align(ui_beepEnableSwitch, LV_ALIGN_CENTER);
    lv_obj_set_style_shadow_color(ui_beepEnableSwitch, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui_beepEnableSwitch, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui_beepEnableSwitch, 33, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui_beepEnableSwitch, 2, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_labelPhantom = lv_label_create(ui_settingsMenu);
    lv_obj_set_width(ui_labelPhantom, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_labelPhantom, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_labelPhantom, -60);
    lv_obj_set_y(ui_labelPhantom, -61);
    lv_obj_set_align(ui_labelPhantom, LV_ALIGN_CENTER);
    lv_label_set_text(ui_labelPhantom, "PHANTOM");

    ui_spkEnableLabel = lv_label_create(ui_settingsMenu);
    lv_obj_set_width(ui_spkEnableLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_spkEnableLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_spkEnableLabel, -53);
    lv_obj_set_y(ui_spkEnableLabel, -3);
    lv_obj_set_align(ui_spkEnableLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_spkEnableLabel, "speaker\nenable");
    lv_obj_set_style_text_font(ui_spkEnableLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_beepEnableLabel = lv_label_create(ui_settingsMenu);
    lv_obj_set_width(ui_beepEnableLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_beepEnableLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_beepEnableLabel, -53);
    lv_obj_set_y(ui_beepEnableLabel, 74);
    lv_obj_set_align(ui_beepEnableLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_beepEnableLabel, "beep\nenable");
    lv_obj_set_style_text_font(ui_beepEnableLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

}
void ui_cableTestScreen_screen_init(void)
{
    ui_cableTestScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_cableTestScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_cableTesterMenu = lv_obj_create(ui_cableTestScreen);
    lv_obj_set_width(ui_cableTesterMenu, 240);
    lv_obj_set_height(ui_cableTesterMenu, 240);
    lv_obj_set_align(ui_cableTesterMenu, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_cableTesterMenu, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_acceptIcon = lv_img_create(ui_cableTesterMenu);
    lv_img_set_src(ui_acceptIcon, &ui_img_accept_png);
    lv_obj_set_width(ui_acceptIcon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_acceptIcon, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_acceptIcon, 0);
    lv_obj_set_y(ui_acceptIcon, 23);
    lv_obj_set_align(ui_acceptIcon, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_acceptIcon, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_acceptIcon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_acceptIcon, 68, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_noCableLabel = lv_label_create(ui_cableTesterMenu);
    lv_obj_set_width(ui_noCableLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_noCableLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_noCableLabel, 3);
    lv_obj_set_y(ui_noCableLabel, -84);
    lv_obj_set_align(ui_noCableLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_noCableLabel, "No cable ");
    lv_obj_set_style_text_font(ui_noCableLabel, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);

}

void ui_init(void)
{
    LV_EVENT_GET_COMP_CHILD = lv_event_register_id();

    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_startScreen_screen_init();
    ui_mixerMenuScreen_screen_init();
    ui_recPlayScreen_screen_init();
    ui_tracklistScreen_screen_init();
    ui_acceptDeclineScreen_screen_init();
    ui_optionsScreen_screen_init();
    ui_cableTestScreen_screen_init();
    lv_disp_load_scr(ui_startScreen);
}
