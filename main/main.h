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

    PLAYER_PLAY=1,

    RECORD_START,

    MONITOR_START,

    WAVEGEN_START,

    CABLE_TEST_START,

    AUDIO_STOP
};

#define FREQ_NOTIFY_MASK 0xFFFF0000

void set_pwm_backlight (uint8_t percent);

#endif //ESP_AUDIOTEST_MAIN_H
