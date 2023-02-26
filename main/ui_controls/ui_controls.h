//
// Created by ilia on 24.2.23..
//

#ifndef ESP_AUDIOTEST_UI_SLOTS_H
#define ESP_AUDIOTEST_UI_SLOTS_H

#include "ui.h"
#include "ui_comp.h"
#include "ui_helpers.h"
#include "button.h"

#define  GUI_REFRESH_DELAY 200

extern void (*currentScreenUpd) (int, button_t*);

void changeScreen (lv_obj_t* screen);
void ui_updateStartScreen (int encoder_delta, button_t* button_event);
void ui_updateMixerScreen (int encoder_delta, button_t* button_event);
void ui_updateWaveGenScreen (int encoder_delta, button_t* button_event);
void ui_updateRecScreen (int encoder_delta, button_t* button_event);
void ui_updatePlayScreen (int encoder_delta, button_t* button_event);
void ui_updateTrackListScreen (int encoder_delta, button_t* button_event);
void ui_updateAcceptDeclineScreen (int encoder_delta, button_t* button_event);
void ui_updateOptionsScreen (int encoder_delta, button_t* button_event);
void ui_updateCableTestScreen (int encoder_delta, button_t* button_event);

void ui_set_battery_bar (int charge);

#endif //ESP_AUDIOTEST_UI_SLOTS_H
