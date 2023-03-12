//
// Created by ilia on 12.3.23..
//

#ifndef ESP_AUDIOTEST_WAV_H
#define ESP_AUDIOTEST_WAV_H
#include <stdint.h>

#define WAVE_HEADER_SIZE (44)

uint32_t wav_get_size (char* filename);

#endif //ESP_AUDIOTEST_WAV_H
