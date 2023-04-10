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

#define TAG "Wav:"

static uint32_t cur_file_size = 0;
static FILE * cur_file = NULL;

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
    printf("%s\n",fname_record);
    cur_file = fopen(fname_record, "a");
    if (cur_file == NULL) printf("error open file for rec\n");
    char wav_header[WAVE_HEADER_SIZE] = {0};
    fwrite(wav_header, 1, WAVE_HEADER_SIZE, cur_file);
    fseek(cur_file, WAVE_HEADER_SIZE, SEEK_SET);

    return 0xff;
}

void wav_save_record (void) {

    if (cur_file == NULL) return;
    fseek(cur_file, 0,SEEK_END);
    size_t sz = ftell(cur_file);
    printf ("wav recorded size %d\n", sz);
    fclose(cur_file);
    fopen(fname_record, "r+");
    
    char wav_header[WAVE_HEADER_SIZE];
    generate_wav_header(wav_header,sz-WAVE_HEADER_SIZE,SAMPLE_RATE);
    fwrite(wav_header, 1, WAVE_HEADER_SIZE, cur_file);
    fclose(cur_file);

}

void wav_delete_record (void) {
    if (cur_file == NULL) return;

    fclose(cur_file);
    unlink(fname_record);
}

uint32_t wav_write_n_bytes (void* buf, uint32_t n){
    if (cur_file == NULL) return 0;
    uint32_t ret = fwrite(buf,1,n,cur_file);
    return ret;
}

uint32_t wav_read_n_bytes (void* buf, uint32_t n) {
    if (cur_file == NULL) return 0;
    uint32_t ret = fread(buf,1,n,cur_file);
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
    uint32_t _byte_rate = (hdr[31]<<24) | (hdr[30]<<16) | (hdr[29]<<8) |(hdr[28]<<0);
    if (_byte_rate != BYTE_RATE) {
        ESP_LOGI(TAG, "Byte rate is not supported");
        return 0;
    }
    uint32_t wav_size = (hdr[43]<<24) | (hdr[42]<<16) | (hdr[41]<8) |(hdr[40]<<0);

    fclose(f);
    return wav_size;
}
