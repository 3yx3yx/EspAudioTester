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
#define BTN_LONG_THRESHOLD (1000)
#define INPUT_ACK_DELAY (60)



typedef struct buttonStruct {
    uint8_t pin;
    bool wasLong;
    bool wasPressed;
    uint8_t counter;
}button_t;

button_t* get_button_event (void);
void button_init (void);
//extern QueueSetHandle_t buttonQueue;

#endif //ESP_AUDIOTEST_BUTTON_H
