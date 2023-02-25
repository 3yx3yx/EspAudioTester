//
// Created by ilia on 13.1.23..
//

#include "encoder.h"
#include "main.h"

#define EC_GPIO_A 34
#define EC_GPIO_B 35

#define PCNT_HIGH_LIMIT (100)
#define PCNT_LOW_LIMIT  (-100)

//QueueSetHandle_t encoderQueue;
pcnt_unit_handle_t enc;


pcnt_unit_handle_t encoderInit (void){
    pcnt_unit_config_t unit_config = {
            .high_limit = PCNT_HIGH_LIMIT,
            .low_limit =  PCNT_LOW_LIMIT,
    };
    pcnt_unit_handle_t pcnt_unit = NULL;
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));


    pcnt_glitch_filter_config_t filter_config = {
            .max_glitch_ns = 1000,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

    pcnt_chan_config_t chan_a_config = {
            .edge_gpio_num = EC_GPIO_A,
            .level_gpio_num = EC_GPIO_B,
    };
    pcnt_channel_handle_t pcnt_chan_a = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_a_config, &pcnt_chan_a));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_HOLD, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
    return pcnt_unit;
}

int encoderGetPulseCount (pcnt_unit_handle_t pcnt_unit){
    int pulse_count = 0;
    ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &pulse_count));
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    return pulse_count;
}

uint16_t encoderCalculatePosition(int pulseCount, int current, int min, int max){
    int pos = current+pulseCount;
    if (pos>max) pos=max;
    if (pos<min) pos=min;
    return pos;
}

void encoderTask (void* arg){
    enc = encoderInit();
//    encoderQueue = xQueueCreate(10,sizeof(int));
//    assert(encoderQueue!=NULL);

    while(1){
        vTaskDelay(pdMS_TO_TICKS(100));
        int pulseCnt = encoderGetPulseCount(enc);
//        xQueueSend(encoderQueue,&pulseCnt, pdMS_TO_TICKS(1000));
        uint32_t event = 0;
        if (pulseCnt<0) {
            event|= 0x80;
        }
        event |= ((uint32_t)abs(pulseCnt) & 0x7F);
        xTaskNotifyAndQuery(guiTaskHandle,event,eSetBits,NULL);
    }
}
/*void encoderTask (void *pvParameter) {
    int position = 0;
    while (1) {

        ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &pulse_count));
        if (pulse_count){
            position += pulse_count;
            if (position>100) { position = 100;}
            if (position<0) { position=0;}

            ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
            ESP_LOGI(TAG, "Position: %d", position);
        }
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}*/
