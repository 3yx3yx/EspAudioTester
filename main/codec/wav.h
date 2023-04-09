//
// Created by ilia on 12.3.23..
//

#ifndef ESP_AUDIOTEST_WAV_H
#define ESP_AUDIOTEST_WAV_H
#include <stdint.h>

#define WAVE_HEADER_SIZE (44)

uint32_t wav_get_size (char* filename);
uint32_t wav_open_file (char* filename);
uint32_t wav_read_n_bytes (void* buf, uint32_t n);
uint8_t wav_rewind (void);

#endif //ESP_AUDIOTEST_WAV_H
