//
// Created by ilia on 13.1.23..
//

#ifndef MAIN_ENCODER_H
#define MAIN_ENCODER_H

#include "driver/pulse_cnt.h"

pcnt_unit_handle_t encoderInit (void);
int encoderGetPulseCount (pcnt_unit_handle_t pcnt_unit);
uint16_t encoderCalculatePosition(int pulseCount, int current, int min, int max);

#endif //MAIN_ENCODER_H
