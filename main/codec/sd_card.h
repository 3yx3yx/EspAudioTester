//
// Created by ilia on 11.3.23..
//

#ifndef ESP_AUDIOTEST_SD_CARD_H
#define ESP_AUDIOTEST_SD_CARD_H

#define SAMPLE_RATE     (44100)
#define NUM_CHANNELS 2
#define BIT_PER_SAMPLE 16
#define BYTE_RATE_44100 (SAMPLE_RATE*NUM_CHANNELS*BIT_PER_SAMPLE/8)
#define BYTE_RATE_48000 (48000*NUM_CHANNELS*BIT_PER_SAMPLE/8)

#define MOUNT_POINT      "/sdcard"
void mount_sdcard(void);
void record_wav(uint32_t rec_time);
void sd_get_file_list (char* list, uint16_t start_n, uint16_t end_n) ;

void get_nth_file_name (uint16_t n, char* name);
uint32_t get_files_count_at_mp (void);

void get_new_record_name (char* name);
void generate_wav_header_mono(char *wav_header, uint32_t wav_size, uint32_t sample_rate);

#endif //ESP_AUDIOTEST_SD_CARD_H
