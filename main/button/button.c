//
// Created by ilia on 24.2.23..
//
#include "button.h"
#include "main.h"

#define BTN_LONG_THRESHOLD (1000)
#define BTN_ACK_DELAY (50)
#define BUTTONS_N (2)

//QueueSetHandle_t buttonQueue;

void buttonTask (void* arg){
    gpio_reset_pin(BTN_ENC_PIN);
    gpio_reset_pin(BTN_LEFT_PIN);
    gpio_set_direction(BTN_ENC_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(BTN_LEFT_PIN, GPIO_MODE_INPUT);

//    buttonQueue = xQueueCreate(5,sizeof(button_t));
//    assert(buttonQueue!=NULL);

    button_t leftBtn = {BTN_LEFT_PIN,0,0,0,BTN_LEFT_IDX};
    button_t encBtn = {BTN_ENC_PIN,0,0,0,BTN_ENC_IDX};
    button_t buttons[BUTTONS_N] = {encBtn,leftBtn};
    uint32_t notifyEvent=0;
    while(1){
        vTaskDelay(pdMS_TO_TICKS(BTN_ACK_DELAY));
        for (uint8_t i=0; i<BUTTONS_N; i++)
        {
            if (!gpio_get_level(buttons[i].pin)) {

                if (buttons[i].wasPressed) {
                    buttons[i].counter++;
                    if (buttons[i].counter > (BTN_LONG_THRESHOLD/BTN_ACK_DELAY)) {
                        buttons[i].wasLong=true;
                        notifyEvent= ((buttons[i].idx)<<8) & BTN_NOTIFY_MASK;
                        notifyEvent |= 1<<11;

                        xTaskNotifyAndQuery(guiTaskHandle, notifyEvent, eSetBits, NULL);
                        buttons[i].wasPressed=false;
                        buttons[i].counter=0;
                    }
                }
                buttons[i].wasPressed=true;
            }
            else {
                if (buttons[i].wasPressed) {
                    notifyEvent = ((buttons[i].idx)<<8) & BTN_NOTIFY_MASK;
                    xTaskNotifyAndQuery(guiTaskHandle, notifyEvent, eSetBits, NULL);
                }
                buttons[i].wasPressed=false;
                buttons[i].counter=0;
            }
        }

    }
}