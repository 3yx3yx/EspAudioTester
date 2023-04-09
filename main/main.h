//
// Created by ilia on 23.2.23..
//

#ifndef ESP_AUDIOTEST_MAIN_H
#define ESP_AUDIOTEST_MAIN_H

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

enum { // enum with signals from UI to playback task
    PLAYER_STOP=1,
    PLAYER_PLAY,
};

void set_pwm_backlight (uint8_t percent);
void set_vu_slider (int val);

#endif //ESP_AUDIOTEST_MAIN_H
