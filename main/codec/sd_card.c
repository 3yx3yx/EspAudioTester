//
// Created by ilia on 11.3.23..
//
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_system.h"
#include "esp_vfs_fat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/sdmmc_host.h"
#include "driver/gpio.h"
#include <dirent.h>
#include "sdmmc_cmd.h"
#include "sdkconfig.h"

#include "sd_card.h"


const int WAVE_HEADER_SIZE = 44;

#define TRACK_NAME_MAX_LEN (32)
#define WAVE_FREQ_HZ    (100)
#define PI              (3.14159265)
#define SAMPLE_PER_CYCLE (SAMPLE_RATE/WAVE_FREQ_HZ)
#define EXAMPLE_BUFF_SIZE (SAMPLE_PER_CYCLE*2)


static const char *TAG = "sdcard";
sdmmc_host_t host = SDMMC_HOST_DEFAULT();
sdmmc_card_t *card;

static uint32_t files_n_at_mountpoint=0;

uint32_t get_files_count_at_mp (void) {
    return files_n_at_mountpoint;
}

void mount_sdcard(void)
{
    esp_err_t ret;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
            .format_if_mount_failed = true,
            .max_files = 5,
            .allocation_unit_size = 16 * 1024
    };
    const char mount_point[] = MOUNT_POINT;
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 4;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                          "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                          "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }
    ESP_LOGI(TAG, "Filesystem mounted");
    sdmmc_card_print_info(stdout, card);

    DIR *d;
    struct dirent *dir;
    d = opendir(MOUNT_POINT);

    if (d) {
        while((dir = readdir(d))!=NULL) {
            files_n_at_mountpoint++;
        }
    }

    /*// First create a file.
    const char *file_hello = MOUNT_POINT"/hello.txt";

    ESP_LOGI(TAG, "Opening file %s", file_hello);
    FILE *f = fopen(file_hello, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "Hello %s!\n", card->cid.name);
    fclose(f);
    ESP_LOGI(TAG, "File written");

    const char *file_foo = MOUNT_POINT"/foo.txt";

    // Check if destination file exists before renaming
    struct stat st;
    if (stat(file_foo, &st) == 0) {
        // Delete it if it exists
        unlink(file_foo);
    }

    // Rename original file
    ESP_LOGI(TAG, "Renaming file %s to %s", file_hello, file_foo);
    if (rename(file_hello, file_foo) != 0) {
        ESP_LOGE(TAG, "Rename failed");
        return;
    }

    // Open renamed file for reading
    ESP_LOGI(TAG, "Reading file %s", file_foo);
    f = fopen(file_foo, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }

    // Read a line from file
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);

    // Strip newline
    char *pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);*/
}

void generate_wav_header(char *wav_header, uint32_t wav_size, uint32_t sample_rate)
{
    // See this for reference: http://soundfile.sapp.org/doc/WaveFormat/
    uint32_t file_size = wav_size + WAVE_HEADER_SIZE - 8;
    uint32_t byte_rate = BYTE_RATE;

    const char set_wav_header[] = {
            'R', 'I', 'F', 'F', // ChunkID
            file_size, file_size >> 8, file_size >> 16, file_size >> 24, // ChunkSize
            'W', 'A', 'V', 'E', // Format
            'f', 'm', 't', ' ', // Subchunk1ID
            0x10, 0x00, 0x00, 0x00, // Subchunk1Size (16 for PCM)
            0x01, 0x00, // AudioFormat (1 for PCM)
            0x02, 0x00, // NumChannels (2 channel)
            sample_rate, sample_rate >> 8, sample_rate >> 16, sample_rate >> 24, // SampleRate
            byte_rate, byte_rate >> 8, byte_rate >> 16, byte_rate >> 24, // ByteRate
            0x04, 0x00, // BlockAlign
            0x10, 0x00, // BitsPerSample (16 bits)
            'd', 'a', 't', 'a', // Subchunk2ID
            wav_size, wav_size >> 8, wav_size >> 16, wav_size >> 24, // Subchunk2Size
    };

    memcpy(wav_header, set_wav_header, sizeof(set_wav_header));
}

void record_wav(uint32_t rec_time)
{
    // Use POSIX and C standard library functions to work with files.
    int flash_wr_size = 0;
    ESP_LOGI(TAG, "Opening file");

    char wav_header_fmt[WAVE_HEADER_SIZE];
    uint32_t wav_size = BYTE_RATE * rec_time;
    wav_size = EXAMPLE_BUFF_SIZE*2*1000*2;
    generate_wav_header(wav_header_fmt, wav_size, SAMPLE_RATE);

    // First check if file exists before creating a new file.
    struct stat st;
    if (stat(MOUNT_POINT"/record.wav", &st) == 0) {
        // Delete it if it exists
        unlink(MOUNT_POINT"/record.wav");
    }

    // Create new WAV file
    FILE *f = fopen(MOUNT_POINT"/record.wav", "a");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }

    // Write the header to the WAV file
    fwrite(wav_header_fmt, 1, WAVE_HEADER_SIZE, f);

    uint16_t samples [EXAMPLE_BUFF_SIZE] = {};
    // Start recording
    double angle = 0.00;
    for (int i = 0; i < EXAMPLE_BUFF_SIZE; i+=2) {
        samples[i] = (uint16_t) (16000 * sin(angle));
        samples[i+1] = (uint16_t) (16000 * sin(angle));
        angle = (2*PI*i)/(float)EXAMPLE_BUFF_SIZE;
    }

    while (flash_wr_size < wav_size) {
        fwrite(samples, 2, EXAMPLE_BUFF_SIZE, f);
        flash_wr_size += EXAMPLE_BUFF_SIZE*2*2;
    }
    ESP_LOGI(TAG, "Recording done!");
    fclose(f);
}


void sd_get_file_list (char* list, uint16_t start_n, uint16_t end_n) {
    DIR *d;
    struct dirent *dir;
    d = opendir(MOUNT_POINT);
    if (list == NULL || end_n <= start_n) {
        ESP_LOGW(TAG,"getlist wrong param");
        return;
    }

    list[0] = '\0';
    if (d) {
        seekdir(d, start_n);

        uint16_t i = 0;
        while ((dir = readdir(d)) != NULL && (i < (end_n - start_n))) {
            i++;
            uint16_t name_len = strlen(dir->d_name);
            if (name_len > TRACK_NAME_MAX_LEN) { // display only first 32 chars
                name_len = TRACK_NAME_MAX_LEN;
            }
            char track_name[TRACK_NAME_MAX_LEN + 2] = {}; // add place for 2 symbols \n and \0
            strncpy(track_name, dir->d_name, name_len);
            track_name[name_len] = '\n';  // insert new string symbol if it is not the last filename
            track_name[name_len + 1] = '\0';  // insert terminator
            strcat(list, track_name); // now we can safely use strcat
        }

        if (strlen(list)>1) {list[strlen(list)-1] = '\0';}

        closedir(d);
    }
}

void get_new_record_name (char* name) {
    if (name == NULL) return;
    name[0] = '\0';
    uint16_t record_idx = 0;
    struct stat st;
    char tmp_name[255+1] = "";
    do{

        tmp_name[0] = '\0';
        sprintf(tmp_name,MOUNT_POINT"/Record_%d.wav", record_idx);
        if (++record_idx == 1000) return;

    } while (stat(tmp_name, &st) == 0);

    stpcpy(name,tmp_name);

}

void get_nth_file_name (uint16_t n, char* name) {
    if (name == NULL) {
        ESP_LOGE (TAG, "NULL FNAME");
        return;
    }
    sprintf (name,MOUNT_POINT"/");
    DIR *d;
    struct dirent *dir;
    d = opendir(MOUNT_POINT);

    if (d) {
        seekdir(d, n);
        dir = readdir(d);
        strcat (name, dir->d_name);
    }

}

