//
// Created by ilia on 24.2.23..
//

#ifndef ESP_AUDIOTEST_BUTTON_H
#define ESP_AUDIOTEST_BUTTON_H

#include "freertos/FreeRTOS.h"
//#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BTN_ENC_PIN (33)
#define BTN_LEFT_PIN (39)
#define BTN_ENC_IDX (1)
#define BTN_LEFT_IDX (2)
#define BTN_NOTIFY_MASK (0xF00)
void buttonTask (void* arg);

typedef struct buttonStruct {
    uint8_t pin;
    bool wasLong;
    bool wasPressed;
    uint8_t counter;
    uint8_t idx;
}button_t;

//extern QueueSetHandle_t buttonQueue;

#endif //ESP_AUDIOTEST_BUTTON_H
