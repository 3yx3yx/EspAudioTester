//
// Created by ilia on 13.1.23..
//

#ifndef MAIN_ENCODER_H
#define MAIN_ENCODER_H

#include "driver/pulse_cnt.h"
#include "freertos/FreeRTOS.h"
//#include "freertos/queue.h"
#include "freertos/task.h"

#define ENCODER_NOTIFY_MASK (0xFF)

pcnt_unit_handle_t encoderInit (void);
int encoderGetPulseCount (pcnt_unit_handle_t pcnt_unit);
uint16_t encoderCalculatePosition(int pulseCount, int current, int min, int max);

void encoderTask (void* arg);
//extern QueueSetHandle_t encoderQueue;

#endif //MAIN_ENCODER_H
