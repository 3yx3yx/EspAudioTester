//
// Created by ilia on 12.3.23..
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_log.h"
#include "esp_err.h"
#include "sd_card.h"
#include <dirent.h>
#include "wav.h"
#include <unistd.h>
#include "wm8960.h"

#define TAG "Wav:"

static uint32_t cur_file_size = 0;
static FILE * cur_file = NULL;
static uint8_t curr_n_channels = 2;

int fdesc = 0;

static uint16_t f_buf [N_FRAMES] = {}; // buffer for writing stereo i2s to mono wav file


uint32_t wav_open_file (char* filename) {
    if (cur_file != NULL) {
        fclose(cur_file);
    }
    cur_file_size = wav_get_size(filename);
    if (cur_file_size) {
        cur_file = fopen(filename, "rw");
    }
    fseek(cur_file, WAVE_HEADER_SIZE, SEEK_SET);
    return cur_file_size;
}

char fname_record[0xFF] = "";

uint8_t wav_start_record (void) {
    fname_record[0] = '\0';
    get_new_record_name(fname_record);
    if (strlen(fname_record) == 0) return 0;
    printf("rec %s\n",fname_record);

    cur_file = fopen(fname_record, "a");

    if (cur_file == NULL){
        printf("error open file for rec\n");
        return 0;
    }
    fdesc = fileno(cur_file);

    char wav_header[WAVE_HEADER_SIZE] = {0};
    fwrite(wav_header, 1, WAVE_HEADER_SIZE, cur_file);

    return 0xff;
}

extern uint32_t files_n_at_mountpoint;

void wav_save_record (void) {

    if (cur_file == NULL) return;
    fseek(cur_file, 0,SEEK_END);
    size_t sz = ftell(cur_file);
    printf ("wav recorded size %d\n", sz);
    fclose(cur_file);
    fopen(fname_record, "r+");

    char wav_header[WAVE_HEADER_SIZE];
    generate_wav_header_mono(wav_header, sz - WAVE_HEADER_SIZE, SAMPLE_RATE);
    fwrite(wav_header, 1, WAVE_HEADER_SIZE, cur_file);
    fclose(cur_file);

    ++files_n_at_mountpoint;

}

void wav_delete_record (void) {
    if (cur_file == NULL) return;

    fclose(cur_file);
    unlink(fname_record);
}

/// @brief writes stereo i2s stream to mono wav file
/// \param buf
/// \param n
/// \return written bytes n
uint32_t wav_write_n_bytes (void* buf, uint32_t n) {
    if (cur_file == NULL || fdesc == 0) return 0;
    uint16_t *p = (uint16_t*)buf;
    for (int i=0; i < n; i+=2) {
        f_buf[i / 2] = p[i]; // copy each 2nd sample
    }
    uint32_t ret = write(fdesc, f_buf, n/2);
    return ret;
}

uint32_t wav_read_n_bytes (void* buf, uint32_t n) {
    if (cur_file == NULL) return 0;
    uint32_t ret = 0;
    if (curr_n_channels == 2) {
        ret = fread(buf,1,n,cur_file);
    } else {
        if (n > N_FRAMES * sizeof (uint16_t)) n = N_FRAMES*sizeof (uint16_t);

        ret = fread(f_buf, 1, n/2, cur_file);

        uint16_t *p = (uint16_t*)buf;
        uint16_t  smp = 0;
        for (int i = 0; i < n/2; i++) {  // mono to stereo

            if (i%2 == 0)  smp = f_buf[i/2];
            p[i] = smp;

        }

        ret *=2;

    }

    return ret;
}

uint8_t wav_rewind (void) {
    if (cur_file == NULL) return 0;
    rewind(cur_file);
    fseek(cur_file, WAVE_HEADER_SIZE, SEEK_SET);

    return 0xFF;
}

uint32_t wav_get_size (char* filename) {
    FILE* f;

    f = fopen(filename, "r");
    printf(filename);
    printf(" \n");
    if (f == NULL) {
        ESP_LOGI(TAG, "FAILED to open!");
        return 0;
    }

    char hdr[WAVE_HEADER_SIZE] = "";
    fread(hdr,1,4,f); // read first 4 chars
    if (strcmp(hdr, "RIFF") != 0 ) {
        ESP_LOGI(TAG, "FILE is NOT WAV!");
        return 0;
    }

    rewind(f);

    fread(hdr,1,WAVE_HEADER_SIZE,f);

    uint32_t byteRate = (hdr[31] << 24) | (hdr[30] << 16) | (hdr[29] << 8) | (hdr[28] << 0);
    uint8_t n_chan = hdr[22];
    uint8_t block_align = hdr[32];
    uint32_t wav_size = (hdr[43]<<24) | (hdr[42]<<16) | (hdr[41]<8) |(hdr[40]<<0);

    if (byteRate == BYTE_RATE_44100/2 && n_chan == 1 && block_align == 0x2) {
        curr_n_channels = 1;
        i2s_set_sample_rates(I2S_NUM_0, 44100);
        printf("file is mono\n");
    } else if (byteRate == BYTE_RATE_44100 && n_chan == 0x2 && block_align == 0x4){
        curr_n_channels = 2;
        i2s_set_sample_rates(I2S_NUM_0, 44100);
        printf("file is stereo\n");
    } else if (byteRate == BYTE_RATE_48000 && n_chan == 0x2 && block_align == 0x4){
        curr_n_channels = 2;
        i2s_set_sample_rates(I2S_NUM_0, 48000);
        printf("file is stereo\n");
    }else {
        printf ("wav fmt err\n");
        return  0;
    }


    fclose(f);
    printf ("wav size %d\n", wav_size);
    return wav_size;
}

uint8_t wav_curr_file_get_n_channels (void) {
    return curr_n_channels;
}

void wav_close_current (void){
    if (cur_file != NULL)
        fclose(cur_file);
}