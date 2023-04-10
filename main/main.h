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

enum { // enum with signals from UI to audio tasks
    PLAYER_STOP=1,
    PLAYER_PLAY,
    RECORD_START,
    RECORD_STOP
};

void set_pwm_backlight (uint8_t percent);

#endif //ESP_AUDIOTEST_MAIN_H
