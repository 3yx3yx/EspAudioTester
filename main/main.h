//
// Created by ilia on 23.2.23..
//

#ifndef ESP_AUDIOTEST_MAIN_H
#define ESP_AUDIOTEST_MAIN_H

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern  TaskHandle_t guiTaskHandle;
void guiTask(void *pvParameter);

#endif //ESP_AUDIOTEST_MAIN_H
