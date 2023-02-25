//
// Created by ilia on 24.2.23..
//
#include "button.h"
#include "main.h"


#define BUTTONS_N (2)

//QueueSetHandle_t buttonQueue;
button_t leftBtn = {BTN_LEFT_PIN,0,0,0};
button_t encBtn =  {BTN_ENC_PIN,0,0,0};
button_t* buttons[BUTTONS_N] = {&encBtn,&leftBtn};

void button_init (void) {
    ESP_ERROR_CHECK(gpio_reset_pin(BTN_ENC_PIN));
    ESP_ERROR_CHECK(gpio_reset_pin(BTN_LEFT_PIN));
    ESP_ERROR_CHECK(gpio_set_direction(BTN_ENC_PIN, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_direction(BTN_LEFT_PIN, GPIO_MODE_INPUT));
}

button_t* get_button_event (void) {
    for (uint8_t i=0; i<BUTTONS_N; i++)
    {
        if (!gpio_get_level(buttons[i]->pin)) {
            if (buttons[i]->wasPressed) {
                buttons[i]->counter++;
                if (buttons[i]->counter == (BTN_LONG_THRESHOLD/INPUT_ACK_DELAY)) {
                    buttons[i]->wasLong=true;
                    buttons[i]->wasPressed=false;
                    return buttons[i];
                }
            }
            buttons[i]->wasPressed=true;
        }
        else {
            buttons[i]->counter=0;
            buttons[i]->wasLong=false;
            if (buttons[i]->wasPressed) {
                buttons[i]->wasPressed=false;
                return buttons[i];
            }
        }
    }
    return NULL;
}


