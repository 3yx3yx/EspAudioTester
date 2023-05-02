//
// Created by ilia on 13.1.23..
//

#include "wm8960.h"
#include "math.h"
#include "freertos/queue.h"
#include "nvs.h"
#include "esp_log.h"
#include "soc/io_mux_reg.h"

#define SAMPLE_RATE     (44100)
#define WAVE_FREQ_HZ    (1000)
#define PI              (3.14159265)
#define I2S_BCK_IO      (19)

#define I2S_WS_IO       (16)
#define I2S_DO_IO       (17)
#define I2S_DI_IO       (5)

#define I2C_SDA (21)
#define I2C_SCL (22)

#define SAMPLE_PER_CYCLE (SAMPLE_RATE/WAVE_FREQ_HZ)
#define BUFFERS_PER_SEC ((SAMPLE_RATE*2)/N_FRAMES)

static uint16_t samples[N_DESC][N_FRAMES] = {};

void i2c_init(void) {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_SDA;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_SCL;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;
    conf.clk_flags = 0;
    ESP_ERROR_CHECK(i2c_param_config(0, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(0, conf.mode, 0, 0, 0));
}

void i2s_init(void) {
    i2s_config_t i2s_config = {
            .mode = I2S_MODE_MASTER | I2S_MODE_TX  | I2S_MODE_RX,
            .sample_rate = 44100,
            .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
            .communication_format = I2S_COMM_FORMAT_STAND_MSB,
            .tx_desc_auto_clear = true,
            .dma_buf_count = N_DESC,
            .dma_buf_len = N_FRAMES,
            .use_apll = 0,
            .intr_alloc_flags = 0 // Interrupt level 1, default 0
    };

    static const i2s_pin_config_t pin_config = {

            .bck_io_num = I2S_BCK_IO,
            .ws_io_num = I2S_WS_IO,
            .data_out_num = I2S_DO_IO,
            .data_in_num = I2S_DI_IO
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);   //install and start i2s driver
    i2s_set_pin(I2S_NUM_0, &pin_config);

    REG_WRITE(PIN_CTRL, 0xFF0);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
}


esp_err_t wm8960_writeReg(uint8_t reg, uint16_t dat) {
    uint8_t I2C_Data[2];
    I2C_Data[0] = (reg<<1)|((uint8_t)((dat>>8)&0x0001));  //RegAddr
    I2C_Data[1] = (uint8_t)(dat&0x00FF);                  //RegValue
    esp_err_t ret = 0;

    uint8_t cnt = 0;

    do {
        ret = i2c_master_write_to_device(I2C_NUM_0, WM8960_ADDRESS, I2C_Data, 2, pdMS_TO_TICKS(100));
        vTaskDelay(pdMS_TO_TICKS(30));

        printf("i2c reg=%d ret = %d data0="BYTE_TO_BINARY_PATTERN
            " data 1="BYTE_TO_BINARY_PATTERN"\n",reg, ret,
            BYTE_TO_BINARY(I2C_Data[0]), BYTE_TO_BINARY(I2C_Data[1]));

        ++cnt;

    } while (ret != ESP_OK && cnt < 10);


    return ret;
}

extern TaskHandle_t i2s_task_handle;
extern TaskHandle_t wav_task_handle;
extern TaskHandle_t main_task_handle;
const uint32_t i2s_msg_mask = 0xFF00;


void i2s_task(void *args) {

    i2c_init();
    wm8960Init();
    i2s_init();
    gpio_expander_init ();

    esp_err_t ret = ESP_OK;
    size_t bytes_written_now = 0;
    volatile uint8_t idx = 0;
    uint32_t notif = 0;
    uint8_t idx_max = 0;

    vTaskSuspend(i2s_task_handle);

    while (1) {

        if (xTaskNotifyWait(0, ULONG_MAX, &notif, 10) == pdTRUE) { // check the last updated buffer index
            idx_max = notif & 0xFF;
        }

        if (idx != idx_max) {
            ret = i2s_write(I2S_NUM_0, &samples[idx][0], N_FRAMES * sizeof(uint16_t), &bytes_written_now,
                            portMAX_DELAY);
        }

        xTaskNotify(wav_task_handle, ((idx + 1) << 8) & i2s_msg_mask,
                    eSetValueWithOverwrite); // notify the wav task where i2s task stopped

        if (ret != ESP_OK) { printf("i2s fail\n"); }

        ++idx;
        if (idx == N_DESC) { idx = 0; }

        portYIELD();
    }
}


extern TaskHandle_t file_task_handle;
extern QueueHandle_t freq_queue;

void wav_task(void *args) {

    volatile size_t n_bytes_from_file = 0;
    uint32_t notif = 0;
    const uint32_t ui_msg_mask = 0xFF; // message from ui
    uint8_t last_idx = 0;
    bool restart = 0;
    uint64_t peak_sum = 0;
    uint16_t buffers_cnt = 0;
    uint8_t i2s_last_idx = 0;



    while (1) {

        xTaskNotifyWait(0, ULONG_MAX, &notif, portMAX_DELAY);

        if ((notif & ui_msg_mask) == PLAYER_PLAY || restart) {
            if (restart) printf("RESTART\n");
            restart = true;
            printf("wav task play started\n");

            vTaskResume(i2s_task_handle);

            for (last_idx = 0; last_idx < N_DESC; ++last_idx) { // fill the buffers before starting i2s write
                wav_read_n_bytes(&samples[last_idx][0], (N_FRAMES) * sizeof(uint16_t));
            }
            xTaskNotify(i2s_task_handle, last_idx - 1,
                        eSetValueWithOverwrite); // notify the i2s task which buffer is the latest
            do {
                xTaskNotifyWait(0, ULONG_MAX, &notif,
                                portMAX_DELAY); // wait if some buffer was sent to i2s or other messages
                if ((notif & ui_msg_mask) == AUDIO_STOP) {
                    printf("wav stopped from gui\n");
                    restart = false;
                    vTaskSuspend(i2s_task_handle);
                    break; // stop the file reading
                } else if ((notif & i2s_msg_mask)) { // if msg from i2s task

                    i2s_last_idx = ((notif & i2s_msg_mask) >> 8) - 1; // take last transmitted buffer index

                    do { // fill the buffers until we will fill the last transmitted buffer

                        ++last_idx;
                        if (last_idx >= N_DESC) { last_idx = 0; }
                        n_bytes_from_file = wav_read_n_bytes(&samples[last_idx][0], (N_FRAMES) * sizeof(uint16_t));
                        ++buffers_cnt;
                        for (int i = 0; i < N_FRAMES; i++) {  // calc average sample value
                            peak_sum += samples[last_idx][i];
                        }

                    } while (last_idx != i2s_last_idx);
                    //  notify the i2s task which buffer is the latest and what is the work mode
                    xTaskNotify(i2s_task_handle, last_idx, eSetValueWithOverwrite);
                    // notify main gui task each 1 sec to upd elapsed time and send average audio amplitude
                    if (buffers_cnt >= BUFFERS_PER_SEC) {
                        peak_sum /= N_FRAMES * buffers_cnt;
                        float val = (float) peak_sum / UINT16_MAX;
                        if ((val = 100 + 2 * 20 * logf(val)) < 0) val = 0;
                        xTaskNotify(main_task_handle, val, eSetValueWithOverwrite);
                        buffers_cnt = peak_sum = 0;
                    }
                    portYIELD();
                }
            } while (n_bytes_from_file);


            if (restart) {
                wav_rewind();
            }
            // cleanup the buffers when playback stopped
            i2s_zero_dma_buffer(I2S_NUM_0);
            for (int i = 0; i < N_DESC; i++) {
                for (int j = 0; j < N_FRAMES; j++) {
                    samples[i][j] = 0;
                }
            }
        } else if ((notif & ui_msg_mask) == RECORD_START || (notif & ui_msg_mask) == MONITOR_START) {

            bool monitor = ((notif & ui_msg_mask) == MONITOR_START);
            vTaskSuspend(i2s_task_handle);

            volatile uint16_t seconds = 0;
            const uint16_t timeout_sec = 3600 * 4; // 4 hours
            uint8_t idx = 0;
            esp_err_t ret = 0;
            size_t bytes_n = 0;
            do {
                xTaskNotifyWait(0, ULONG_MAX, &notif, 0); //wait i2s or other messages
                if ((notif & ui_msg_mask) == AUDIO_STOP) {
                    printf("rec stop\n");
                    break;
                }
                ret = i2s_read(I2S_NUM_0, &samples[idx][0], N_FRAMES * sizeof(uint16_t), &bytes_n, 1000);
                if (ret != ESP_OK) printf("i2s fail read\n");

                if (!monitor) {
                    xTaskNotify(file_task_handle, idx, eSetValueWithOverwrite);
                }

                //echo
                ret = i2s_write(I2S_NUM_0, &samples[idx][0], N_FRAMES * sizeof(uint16_t), &bytes_n, 1000);
                if (ret != ESP_OK) printf("i2s fail wr\n");

                ++idx;
                if (idx == N_DESC) idx = 0;

                portYIELD();

            } while ((seconds < timeout_sec));


        } else if ((notif & ui_msg_mask) == WAVEGEN_START || (notif & ui_msg_mask) == CABLE_TEST_START) {
            uint16_t freq = 1000;
            printf (" wavegen start \n");
            double smp = 0;
            uint16_t smp_per_cycle = SAMPLE_RATE / freq;
            double inv_smp_per_cycle = 1.00 / smp_per_cycle;
            uint16_t smp_cnt = 0;
            bool cable_test = (notif & ui_msg_mask) == CABLE_TEST_START;
            //xTaskNotifyStateClear(NULL);

            if (xQueueReceive(freq_queue, &freq, portMAX_DELAY) == pdPASS) {
                printf("freq %d\n", freq);
                smp_per_cycle = SAMPLE_RATE / freq;
                inv_smp_per_cycle = 1.0 / smp_per_cycle;
            }

            vTaskResume(i2s_task_handle);
            xTaskNotify(i2s_task_handle, last_idx, eSetValueWithOverwrite);
            do {


                xTaskNotifyWait(0, ULONG_MAX, &notif,
                                portMAX_DELAY); // wait if some buffer was sent to i2s or other messages

                if ((notif & ui_msg_mask) == AUDIO_STOP) {
                    printf ("wavegen stop from ui\n");
                    vTaskSuspend(i2s_task_handle);
                    break; // stop
                } else if ((notif & i2s_msg_mask)) { // if msg from i2s task

                    i2s_last_idx = ((notif & i2s_msg_mask) >> 8) - 1; // take last transmitted buffer index

                    do { // fill the buffers until we will fill the last transmitted buffer

                        ++last_idx;
                        if (last_idx >= N_DESC) { last_idx = 0; }

                        if (xQueueReceive(freq_queue, &freq, 1) == pdPASS) {
                            printf("freq %d\n", freq);
                            smp_per_cycle = SAMPLE_RATE / freq;
                            inv_smp_per_cycle = 1.0 / smp_per_cycle;
                            smp_cnt = 0;
                        }

                        for (int i = 0; i < N_FRAMES; i += 2) {
                            smp = sin(2*PI * smp_cnt * inv_smp_per_cycle);
                            samples[last_idx][i] = (smp+1)*(16000);
                            samples[last_idx][i+1] = samples[last_idx][i];
                            ++smp_cnt;
                            if (smp_cnt == smp_per_cycle) smp_cnt = 0;
                        }


                    } while (last_idx != i2s_last_idx);

                    //  notify the i2s task which buffer is the latest and what is the work mode
                    xTaskNotify(i2s_task_handle, last_idx, eSetValueWithOverwrite);

                }

                if (cable_test) {

//                    ret = i2s_read(I2S_NUM_0, &samples[buf_idx][0], N_FRAMES * sizeof(uint16_t), &bytes_n, 1000);
//                    if (ret != ESP_OK) printf("i2s fail read\n");
//                    double sq_sum = 0; // sample magnitude squares sum
//                    for (int i = 0; i< N_FRAMES; i++) {
//                        double rms_smp = (float)samples[buf_idx][i] / UINT16_MAX;
//                        sq_sum += rms_smp * rms_smp;
//                    }
//                    double rms = sqrt(sq_sum / N_FRAMES);
//
//                    printf("RMS %f\n", rms);
//
//                    const float error_rate = 0.1f;
//                    const float target = 0.707f;
//                    if ((rms < (target + error_rate)) && (rms > (target - error_rate))) {
//
//                    }

                }

                portYIELD();

            }while(1) ;

            i2s_zero_dma_buffer(I2S_NUM_0);
            for (int i = 0; i < N_DESC; i++) {
                for (int j = 0; j < N_FRAMES; j++) {
                    samples[i][j] = 0;
                }
            }
        } else {
            printf("notif %d\n", notif);
        }

        portYIELD();
    }
}

void file_write_task(void *args) {
    uint32_t notif = 0;

    while (1) {
        xTaskNotifyWait(0, ULONG_MAX, &notif, portMAX_DELAY);

        uint8_t idx = notif & 0xF;

        wav_write_n_bytes(&samples[idx][0], (N_FRAMES) * sizeof(uint16_t));
        portYIELD();
    }
}

///
/// \param percent
/// \return db value
float codec_set_speaker_vol (uint8_t percent) {
    uint16_t reg = 0;
    const uint16_t vol_max = 0b1111111;
    const uint16_t vol_min = 0b0110000;
    const float max_db = 6.00f;
    const float min_db = -73.0f;
    float db = ((max_db - min_db)/100)*(percent) + min_db;

    uint16_t vol = 0;
    if (percent != 0) {
        vol = (uint16_t) (((float) (vol_max - vol_min) / 100.0f) * (percent) + vol_min);
    }

    vol &= vol_max;

/*    R40_LOUT2_VOLUME_t r40Lout2Volume = {
            .SPKLVOL = vol, // left speaker vol 1111 111 =+6db 0110000=-73db 0=mute
            .SPKLZC = 1, // zero cross vol upd
            .SPKVU =1 // update volume
    };
    reg = *(uint16_t *) &r40Lout2Volume;*/

    reg = (1<<8) | (1<<7) | (vol);
    wm8960_writeReg(R40_LOUT2_VOLUME, reg);
    wm8960_writeReg(R41_ROUT2_VOLUME, reg);
    db = roundf(db);
    return db;
}

float codec_set_dac_vol (uint8_t percent){
    uint16_t reg = 0;
    const uint16_t vol_max = 0xFF;
    const uint16_t vol_min = 1;
    const float max_db = 0.00f;
    const float min_db = -127.0f;
    float db = ((max_db - min_db)/100)*(percent) + min_db;
    db = roundf(db);

    uint16_t vol = 0;
    if (percent != 0) {
        vol = (uint16_t) (((float) (vol_max - vol_min) / 100.0f) * (percent) + vol_min);
    }

    vol &= vol_max;

/*    R10_LEFT_DAC_VOLUME_t leftDacVolume = {
            .DACVU = 1, //vol update
            .LDACVOL =vol  // 0b0000 0000 = mute 0b0000 0001 = -127db 1111 1111 = 0db
    };
    reg = *(uint16_t *) &leftDacVolume;*/
    reg = (1<<8) | vol;
    wm8960_writeReg(R10_LEFT_DAC_VOLUME_ADR, reg);
    vTaskDelay(20);
    wm8960_writeReg(R11_RIGHT_DAC_VOLUME_ADR, reg);
    vTaskDelay(20);

    return db;
}

/// headphones are connected to L_out
/// \param percent
/// \return db value
float codec_set_hp_vol (uint8_t percent) {
    uint16_t reg = 0;
    const uint16_t vol_max = 0b1111111;
    const uint16_t vol_min = 0b0110000;
    const float max_db = 6.00f;
    const float min_db = -73.0f;
    float db = ((max_db - min_db)/100)*(percent) + min_db;

    uint16_t vol = 0;
    if (percent != 0) {
        vol = (uint16_t)(((float)(vol_max - vol_min) / 100.0f) * (percent) + vol_min);
    }// 0 = mute

    vol &= vol_max;
//    R2_LOUT1_VOLUME_t lout1Volume = {
//            .OUT1VU = 1, // write 1 to update headphone vol
//            .LO1ZC = 1, // update on zero cross
//            .LOUT1VOL = vol //127 max=+6db, 0110000=-73db, 0 = mute, 1db steps
//    };
//    reg = *(uint16_t *) &lout1Volume;
    reg = (1<<8) | (1<<7) | (vol);
    wm8960_writeReg(R2_LOUT1_VOLUME_ADR, reg);
    vTaskDelay(10);
//    wm8960_writeReg(R3_ROUT1_VOLUME_ADR, reg);
//    vTaskDelay(10);

    db = roundf(db);
    return db;
}

float codec_set_line_out_vol (uint8_t percent) {
    uint16_t reg = 0;
    const uint16_t vol_max = 0b1111111;
    const uint16_t vol_min = 0b0110000;
    const float max_db = 6.00f;
    const float min_db = -73.0f;
    float db = ((max_db - min_db)/100)*(percent) + min_db;

    uint16_t vol = 0;
    if (percent != 0) {
        vol = (uint16_t)(((float)(vol_max - vol_min) / 100.0f) * (percent) + vol_min);
    }// 0 = mute

    vol &= vol_max;
    reg = (1<<8) | (1<<7) | (vol);

    wm8960_writeReg(R3_ROUT1_VOLUME_ADR, reg);
    vTaskDelay(10);

    db = roundf(db);
    return db;
}

float codec_set_mic_gain (uint8_t percent) {

    uint16_t reg = 0;
    const uint16_t vol_max = 0b111111;
    const uint16_t vol_min = 0b00;
    const float max_db = 30.00f;
    const float min_db = -17.25f;
    float db = ((max_db - min_db)/100)*(percent) + min_db;

    uint16_t vol = 0;
    if (percent != 0) {
        vol = (uint16_t)(((float)(vol_max - vol_min) / 100.0f) * (percent) + vol_min);
    }// 0 = mute
    vol &= vol_max;

//    R1_RIGHT_INPUT_VOLUME_t rightInputVolume = {
//            .IPUV = 1, // 1 will update gain
//            .RINMUTE=0, // disable mute
//            .RIZC=1, // update gain on zero cross
//            .RINVOL=0b111000 // default 01011 = 0db 111111=+30db
//    };
    reg = (1<<8) | (1<<6) | (vol);
    wm8960_writeReg(R1_RIGHT_INPUT_VOLUME_ADR, reg);
    db = roundf(db);
    return db;
}

float codec_set_line_in_gain(uint8_t percent) {
    uint16_t reg = 0;
    const uint16_t vol_max = 0b111111;
    const uint16_t vol_min = 0b00;
    const float max_db = 30.00f;
    const float min_db = -17.25f;
    float db = ((max_db - min_db)/100)*(percent) + min_db;

    uint16_t vol = 0;
    if (percent != 0) {
        vol = (uint16_t)(((float)(vol_max - vol_min) / 100.0f) * (percent) + vol_min);
    }// 0 = mute
    vol &= vol_max;
//    R0_LEFT_INPUT_VOLUME_t leftInputVolume = {
//            .IPUV = 1, // 1 will update gain
//            .LINMUTE=0, // disable mute
//            .LIZC=1, // update gain on zero cross
//            .LINVOL= 0b111000 // default 01011 = 0db 111111=+30db
//    };
    reg = (1<<8) | (1<<6) | (vol);
    (wm8960_writeReg(R0_LEFT_INPUT_VOLUME_ADR, reg));
    db = roundf(db);
    return db;
}

void codec_enable_mic_boost (bool state) {
    uint16_t reg = 0;
    if (state) {
        R33_ADCR_SIGNAL_PATH_t adcrSignalPath = {
                .RMIC2B =1,
                .RMICBOOST= 0b10 ,
                .RMN1 =1,
                .RMP2=0,
                .RMP3=0
        };
        reg = *(uint16_t *) &adcrSignalPath;
        ESP_ERROR_CHECK(wm8960_writeReg(R33_ADCR_SIGNAL_PATH, reg));
    } else {
        R33_ADCR_SIGNAL_PATH_t adcrSignalPath = {
                .RMIC2B =1,
                .RMICBOOST= 0b00 ,
                .RMN1 =1,
                .RMP2=0,
                .RMP3=0
        };
        reg = *(uint16_t *) &adcrSignalPath;
        ESP_ERROR_CHECK(wm8960_writeReg(R33_ADCR_SIGNAL_PATH, reg));
    }
}

void codec_enable_line_boost (bool state) {
    uint16_t  reg = 0;
    if (state) {
        R32_ADCL_SIGNAL_PATH_t adclSignalPath = {
                .LMIC2B = 1, //connect amp out to mixer
                .LMICBOOST = 0b10, // 0db, 0b11=+29db see page 23
                .LMN1 = 1, // connect amp input to LINPUT1
                .LMP2 = 0, // LINPUT2
                .LMP3 = 0 // LINPUT3
                //if lmp2 == lmp3 == 0, amp + input is connected to vmid
        };
        reg = *(uint16_t *) &adclSignalPath;
        ESP_ERROR_CHECK(wm8960_writeReg(R32_ADCL_SIGNAL_PATH, reg));
    } else {
        R32_ADCL_SIGNAL_PATH_t adclSignalPath = {
                .LMIC2B = 1, //connect amp out to mixer
                .LMICBOOST = 0b00, // 0db, 0b11=+29db see page 23
                .LMN1 = 1, // connect amp input to LINPUT1
                .LMP2 = 0, // LINPUT2
                .LMP3 = 0 // LINPUT3
                //if lmp2 == lmp3 == 0, amp + input is connected to vmid
        };
        reg = *(uint16_t *) &adclSignalPath;
        ESP_ERROR_CHECK(wm8960_writeReg(R32_ADCL_SIGNAL_PATH, reg));
    }
}

void codec_enable_alc (bool state){
    uint16_t reg = 0;
    if (state) {
        R17_ALC1_t r17Alc1 = {
                /*
                 *  ALC Function Select
                    00 = ALC off (PGA gain set by register)
                    01 = Right channel only
                    10 = Left channel only
                    11 = Stereo (PGA registers unused) Note:
                    ensure that LINVOL and RINVOL settings
                    (reg. 0 and 1) are the same before entering
                    this mode
                 */
                .ALCSEL =0b01,
                /*
                 * Set Maximum Gain of PGA (During ALC
                    operation)
                    111 : +30dB
                    110 : +24dB
                    ....(-6dB steps)
                    001 : -6dB
                    000 : -12dB
                 */
                .MAXGAIN = 0b111,
                /*
                 *  ALC Target (Sets signal level at ADC input)
                    0000 = -22.5dB FS
                    0001 = -21.0dB FS
                    ... (1.5dB steps)
                    1101 = -3.0dB FS
                    1110 = -1.5dB FS
                    1111 = -1.5dB FS
                 */
                .ALCL = 0b1011
        };
        reg = *(uint16_t *) &r17Alc1;
        ESP_ERROR_CHECK(wm8960_writeReg(R17_ALC1_ADR, reg));

        R18_ALC2_t r18Alc2 = {
                /*
                 *  Set Minimum Gain of PGA (During ALC
                    operation)
                    000 = -17.25dB
                    001 = -11.25dB
                    010 = -5.25dB
                    011 = +0.75dB
                    100 = +6.75dB
                    101 = +12.75dB
                    110 = +18.75dB
                    111 = +24.75dB
                 */
                .MINGAIN = 0b000,
                /*
                 *  ALC hold time before gain is increased.
                    0000 = 0ms
                    0001 = 2.67ms
                    0010 = 5.33ms
                    ... (time doubles with every step)
                    1111 = 43.691s
                 */
                .HLD = 0b0000
        };
        reg = *(uint16_t *) &r18Alc2;
        ESP_ERROR_CHECK(wm8960_writeReg(R18_ALC2_ADR, reg));

        R19_ALC3_t r19Alc3 = {
                /*
                 *  Determines the ALC mode of operation:
                    0 = ALC mode
                    1 = Limiter mode
                 */
                .ALCMODE = 0,
                /*
                 *  ALC decay (gain ramp-up) time
                    0000 = 24ms
                    0001 = 48ms
                    0010 = 96ms
                    ... (time doubles with every step)
                    1010 or higher = 24.58s
                 */
                .DCY = 0b0010,
                /*
                 * ALC attack (gain ramp-down) time
                    0000 = 6ms
                    0001 = 12ms
                    0010 = 24ms
                    ... (time doubles with every step)
                    1010 or higher = 6.14s
                 */
                .ATK = 0b0010
        };
        reg = *(uint16_t *) &r19Alc3;
        ESP_ERROR_CHECK(wm8960_writeReg(R19_ALC3_ADR, reg));

        R20_NOISE_GATE_t noiseGate = {
                /*
                 *  Noise gate threshold
                    00000 -76.5dBfs
                    00001 -75dBfs
                    ... 1.5 dB steps
                    11110 -31.5dBfs
                    11111 -30dBfs
                 */
                .NGTH = 0b00000,
                .NGAT = 0 // 0 = noise gate disable
        };
        reg = *(uint16_t *) &noiseGate;
        ESP_ERROR_CHECK(wm8960_writeReg(R20_NOISE_GATE_ADR, reg));
    } else {
        R17_ALC1_t r17Alc1 = {
                /*
                 *  ALC Function Select
                    00 = ALC off (PGA gain set by register)
                    01 = Right channel only
                    10 = Left channel only
                    11 = Stereo (PGA registers unused) Note:
                    ensure that LINVOL and RINVOL settings
                    (reg. 0 and 1) are the same before entering
                    this mode
                 */
                .ALCSEL =0b00,
                /*
                 * Set Maximum Gain of PGA (During ALC
                    operation)
                    111 : +30dB
                    110 : +24dB
                    ....(-6dB steps)
                    001 : -6dB
                    000 : -12dB
                 */
                .MAXGAIN = 0b0000,
                /*
                 *  ALC Target (Sets signal level at ADC input)
                    0000 = -22.5dB FS
                    0001 = -21.0dB FS
                    ... (1.5dB steps)
                    1101 = -3.0dB FS
                    1110 = -1.5dB FS
                    1111 = -1.5dB FS
                 */
                .ALCL = 0b1011
        };
        reg = *(uint16_t *) &r17Alc1;
        ESP_ERROR_CHECK(wm8960_writeReg(R17_ALC1_ADR, reg));

        R18_ALC2_t r18Alc2 = {
                /*
                 *  Set Minimum Gain of PGA (During ALC
                    operation)
                    000 = -17.25dB
                    001 = -11.25dB
                    010 = -5.25dB
                    011 = +0.75dB
                    100 = +6.75dB
                    101 = +12.75dB
                    110 = +18.75dB
                    111 = +24.75dB
                 */
                .MINGAIN = 0b000,
                /*
                 *  ALC hold time before gain is increased.
                    0000 = 0ms
                    0001 = 2.67ms
                    0010 = 5.33ms
                    ... (time doubles with every step)
                    1111 = 43.691s
                 */
                .HLD = 0b0000
        };
        reg = *(uint16_t *) &r18Alc2;
        ESP_ERROR_CHECK(wm8960_writeReg(R18_ALC2_ADR, reg));

        R19_ALC3_t r19Alc3 = {
                /*
                 *  Determines the ALC mode of operation:
                    0 = ALC mode
                    1 = Limiter mode
                 */
                .ALCMODE = 0,
                /*
                 *  ALC decay (gain ramp-up) time
                    0000 = 24ms
                    0001 = 48ms
                    0010 = 96ms
                    ... (time doubles with every step)
                    1010 or higher = 24.58s
                 */
                .DCY = 0b0011,
                /*
                 * ALC attack (gain ramp-down) time
                    0000 = 6ms
                    0001 = 12ms
                    0010 = 24ms
                    ... (time doubles with every step)
                    1010 or higher = 6.14s
                 */
                .ATK = 0b0010
        };
        reg = *(uint16_t *) &r19Alc3;
        ESP_ERROR_CHECK(wm8960_writeReg(R19_ALC3_ADR, reg));

        R20_NOISE_GATE_t noiseGate = {
                /*
                 *  Noise gate threshold
                    00000 -76.5dBfs
                    00001 -75dBfs
                    ... 1.5 dB steps
                    11110 -31.5dBfs
                    11111 -30dBfs
                 */
                .NGTH = 0b00000,
                .NGAT = 0 // 0 = noise gate disable
        };
        reg = *(uint16_t *) &noiseGate;
        ESP_ERROR_CHECK(wm8960_writeReg(R20_NOISE_GATE_ADR, reg));
    }
    // automatic level control

}

float codec_set_alc_max (uint8_t percent){
return 0;
}
void gpio_expander_init (void) {
    uint8_t I2C_Data[2];
    I2C_Data[0] = 0x03; // config register
    I2C_Data[1] = 0; // set all pins as outputs

    esp_err_t ret =
            i2c_master_write_to_device(I2C_NUM_0, GPIO_EXPANDER_ADDR, I2C_Data, 2, pdMS_TO_TICKS(100));

    printf("i2c ret = %d data0="BYTE_TO_BINARY_PATTERN" data 1="BYTE_TO_BINARY_PATTERN"\n", ret, BYTE_TO_BINARY(I2C_Data[0]), BYTE_TO_BINARY(I2C_Data[1]));

}
void gpio_expander_pin_write (uint8_t pin, bool state) {
    if (pin > 7) {
        printf("err i2c expander");
        return;
    }

    static uint8_t outp_reg = 0;
    uint8_t I2C_Data[2];
    I2C_Data[0] = 0x01; // output register

    if (state) {
        outp_reg |= 1U << pin;
    } else {
        outp_reg &= ~(1U << pin);
    }
    I2C_Data[1] = outp_reg;// set all pins as outputs

    esp_err_t ret =
            i2c_master_write_to_device(I2C_NUM_0, GPIO_EXPANDER_ADDR, I2C_Data, 2, pdMS_TO_TICKS(100));

    printf("i2c ret = %d data0="BYTE_TO_BINARY_PATTERN" data 1="BYTE_TO_BINARY_PATTERN"\n", ret, BYTE_TO_BINARY(I2C_Data[0]), BYTE_TO_BINARY(I2C_Data[1]));

}

void enable_phantom (bool state){
    const uint8_t phantom_en_pin = 6;
    gpio_expander_pin_write(phantom_en_pin,state);
}
void enable_atten (bool state){
    const uint8_t atten_relay_set_pin = 3;
    const uint8_t atten_relay_reset_pin = 2;
    if (state) {
        gpio_expander_pin_write(atten_relay_set_pin, 1);
        vTaskDelay(300);
        gpio_expander_pin_write(atten_relay_set_pin, 0);
    } else {
        gpio_expander_pin_write(atten_relay_reset_pin, 1);
        vTaskDelay(300);
        gpio_expander_pin_write(atten_relay_reset_pin, 0);
    }
}
void enable_xlr_swap (bool state){
    const uint8_t xlr_relay_1_set_pin = 1;
    const uint8_t xlr_relay_1_reset_pin = 0;
    const uint8_t xlr_relay_2_set_pin = 4;
    const uint8_t xlr_relay_2_reset_pin = 5;
    if (state) {
        gpio_expander_pin_write(xlr_relay_1_set_pin, 1);
        gpio_expander_pin_write(xlr_relay_2_set_pin, 1);
        vTaskDelay(300);
        gpio_expander_pin_write(xlr_relay_1_set_pin, 0);
        gpio_expander_pin_write(xlr_relay_2_set_pin, 0);
        gpio_expander_pin_write(7,1); // indicator
    } else {
        gpio_expander_pin_write(xlr_relay_1_reset_pin, 1);
        gpio_expander_pin_write(xlr_relay_2_reset_pin, 1);
        vTaskDelay(300);
        gpio_expander_pin_write(xlr_relay_1_reset_pin, 0);
        gpio_expander_pin_write(xlr_relay_2_reset_pin, 0);
        gpio_expander_pin_write(7,0);
    }
}

void wm8960Init() {
    uint16_t reg;

    //wm8960_writeReg(0x0f, 0x0000); // reset
    // power management
    R25_PWR_MGMT_1_t pwrMgmt1 = {
            .ADCL=1,
            .ADCR=1,
            .AINL=1, //PGA
            .AINR=1, //PGA
            .DIGENB=0, // 0 = master clock enabled
            .MICB=1,
            .VMIDSEL = 0b11, // 01 = 50kOhm, 10 = 250k, 11 = 5k divider
            .VREF=1 // necessary for other functions - page 64
    };
    reg = *(uint16_t *) &pwrMgmt1;
    ESP_ERROR_CHECK(wm8960_writeReg(R25_PWR_MGMT_1_ADR, reg));

    R26_PWR_MGMT_2_t pwrMgmt2 = {
            .PLL_EN = 1, // page 66
            .OUT3 = 0,
            .SPKR =1,
            .SPKL=1,
            .ROUT1=1,
            .LOUT1=1,
            .DACL=1,
            .DACR=1
    };
    reg = *(uint16_t *) &pwrMgmt2;
    ESP_ERROR_CHECK(wm8960_writeReg(R26_PWR_MGMT_2_ADR, reg));

    R47_PWR_MGMT_3_t pwrMgmt3 = {
            .LMIC=1,
            .RMIC=1,
            .LOMIX=1,
            .ROMIX=1
    };
    reg = *(uint16_t *) &pwrMgmt3;
    ESP_ERROR_CHECK(wm8960_writeReg(R47_PWR_MGMT_3, reg));
    // end pwr management
    // PLL config
    R52_PLL_N_t r52PllN = {
            /*
             * SYSCLK Output to GPIO Clock Division ratio
            000 = SYSCLK
            001 = SYSCLK / 2
            010 = SYSCLK / 3
            011 = SYSCLK / 4
            100 = SYSCLK / 5.5
            101 = SYSCLK / 6
             */
            .OPCLKDIV = 0b000,
            /*
             * Enable Integer Mode
            0 = Integer mode
            1 = Fractional mode
             */
            .SDM = 0,
            /*
             * Divide MCLK by 2 before input to PLL
            0 = Divide by 1
            1 = Divide by 2
             */
            .PLLRESCALE = 0, /// for mclk 24mhz
            /*
             * Integer (N) part of PLL input/output frequency
            ratio. Use values greater than 5 and less than
            13
             */
            .PLLN = 0x8 // 0x7 for 44.1k
    };
    reg = *(uint16_t *) &r52PllN;
    ESP_ERROR_CHECK(wm8960_writeReg(R52_PLL_1_ADDR, reg));

    R53_PLL_K_1_t r53PllK1 = {
            /*
             * Fractional (K) part of PLL1 input/output
            frequency ratio (treat as one 24-digit binary
            number).
             */
            .PLLK = 0x86// 86h for 44,1k
    };
    reg = *(uint16_t *) &r53PllK1;
    ESP_ERROR_CHECK(wm8960_writeReg(R53_PLL_2_ADDR, reg));

    R54_PLL_K_2_t r54PllK2 = {
            /*
             *  Fractional (K) part of PLL1 input/output
                frequency ratio (treat as one 24-digit binary
                number)
             */
            .PLLK = 0xC2 // c2h 44.1k
    };
    reg = *(uint16_t *) &r54PllK2;
    ESP_ERROR_CHECK(wm8960_writeReg(R54_PLL_3_ADDR, reg));

    R55_PLL_K_3_t r55PllK3 = {
            /*
             *  Fractional (K) part of PLL1 input/output
                frequency ratio (treat as one 24-digit binary number)
             */
            .PLLK = 0x26  // 26h for 44.1k
    };
    reg = *(uint16_t *) &r55PllK3;
    ESP_ERROR_CHECK(wm8960_writeReg(R55_PLL_4_ADDR, reg));
    //CLOCKING CONFIG
    // CLOCKING
    R4_CLOCKING_1_t clocking1 = {  //page 69
            /*
             *  DC Sample rate divider (Also determines
                ADCLRC in master mode)
                000 = SYSCLK / (1.0 * 256)
                001 = SYSCLK / (1.5 * 256)
                010 = SYSCLK / (2 * 256)
                011 = SYSCLK / (3 * 256)
                100 = SYSCLK / (4 * 256)
                101 = SYSCLK / (5.5 * 256)
                110 = SYSCLK / (6 * 256)
                111 = Reserved
             */
            .ADCDIV = 0b000, // see table at page 61 datasheet
            .DACDIV = 0b000,
            /*
             *  SYSCLK Pre-divider. Clock source (MCLK or
                PLL output) will be divided by this value to
                generate SYSCLK.
                00 = Divide SYSCLK by 1
                01 = Reserved
                10 = Divide SYSCLK by 2
                11 = Reserved
             */
            .SYSCLKDIV = 0b00, // 0b10 for 48/44.1khz
            /*
             *  SYSCLK Selection
                0 = SYSCLK derived from MCLK
                1 = SYSCLK derived from PLL output
             */
            .CLKSEL = 0
    };
    reg = *(uint16_t *) &clocking1;
    ESP_ERROR_CHECK(wm8960_writeReg(R4_CLOCKING_1_ADR, reg));

    R8_CLOCKING_2_t clocking2 = {
            .BCLKDIV = 0b100, // bclk freq (master mode)
            .DCLKDIV = 0b111 // 0b111 = sysclk/16 0b110 = sysclk/12 101 = /8 CLASS D CLK
    };
    reg = *(uint16_t *) &clocking2;
    ESP_ERROR_CHECK(wm8960_writeReg(R8_CLOCKING_2_ADR, reg));



    codec_enable_alc(true);
    codec_enable_mic_boost(true);
    codec_enable_line_boost(false);
    codec_set_hp_vol(HP_LINE_VOL_DEFAULT);
    codec_set_line_out_vol(HP_LINE_VOL_DEFAULT);
    codec_set_speaker_vol(SPK_VOL_DEFAULT);
    codec_set_dac_vol(DAC_VOL_DEFAULT);
    codec_set_line_in_gain(50);
    codec_set_mic_gain(50);

    R5_ADC_DAC_CONTROL_CTR1_t adcDacControlCtr1 = {
            .DACDIV2 = 0, // dac attenuator -6db 0=disable
            .ADCPOL = 0b00, // adc polarity non-inverted
            .DACMU = 0, // dac mute = 1
            .DEEMPH = 0b10, // 11 = 48kHz 10 = 44kHz 01=32kHz 00 = off
            .ADCHPD = 0 // 1 = disable highpass filter
    };
    reg = *(uint16_t *) &adcDacControlCtr1;
    ESP_ERROR_CHECK(wm8960_writeReg(R5_ADC_DAC_CONTROL_CTR1_ADR, reg));

    R6_ADC_DAC_CONTROL_CTR2_t adcDacControlCtr2 = { //page 70
            .DACPOL = 0b00, // non-inverted
            .DACSMM = 0, // soft mute 1 = volume will change gradually
            .DACMR = 0, // 1 = slow volume changing ramp 171ms max
            .DACSLOPE = 0 // 0=normal, 1 = sloping stopband
    };
    reg = *(uint16_t *) &adcDacControlCtr2;
    ESP_ERROR_CHECK(wm8960_writeReg(R6_ADC_DAC_CONTROL_CTR2_ADR, reg));

    R7_AUDIO_INTERFACE_t r7AudioInterface = {
            .ALRSWAP = 0, // 0= normal, 1= swap left right data
            .BCLKINV = 0, // BCLK non-inverted 0
            .MS = 0, // 0= slave mode
            .DLRSWAP = 0, // dac l/r swap
            .LRP = 0, // normal lrclk polarity 0
            /*
             *  Audio Data Word Length
                00 = 16 bits
                01 = 20 bits
                10 = 24 bits
                11 = 32 bits (see Note)
             */
            .WL = 0b00, // word len
            /*
             *  00 = Right justified
                01 = Left justified
                10 = I2s
                11 = DSP Mode
             */
            .FORMAT = 0b00//
    };
    reg = *(uint16_t *) &r7AudioInterface;
    ESP_ERROR_CHECK(wm8960_writeReg(R7_AUDIO_INTERFACE_1_ADR, reg));



    R9_AUDIO_INTERFACE_t r9AudioInterface = {
            /*
             *  ADCLRC/GPIO1 Pin Function Select
                0 = ADCLRC frame clock for ADC
                1 = GPIO pin
             */
            .ALRCGPIO = 0,
            /*
             *  8-Bit Word Length Select (Used with
                companding)
                0 = Off
                1 = Device operates in 8-bit mode.
             */
            .WL8 = 0,
            /*
             *  DAC companding
                00 = off
                01 = reserved
                10 = μ-law
                11 = A-law
             */
            .DACCOMP = 0b00,
            .ADCCOMP = 0b00,
            /*
             *  Digital Loopback Function
                0 = No loopback.
                1 = Loopback enabled, ADC data output is fed
                directly into DAC data input.
             */
            .LOOPBACK = 0
    };
    reg = *(uint16_t *) &r9AudioInterface;
    ESP_ERROR_CHECK(wm8960_writeReg(R9_AUDIO_INTERFACE_ADR, reg));

    R16_3D_CONTROL_t r163DControl = {
            .D3DEPTH = 0b1111, // 3d effect 0b1111= 100% 3d effect
            .D3EN = 0, //0=disabled
            .D3LC = 0, // 0 for fs>32kHz, 1 for<32k
            .D3UC = 0,
    };
    reg = *(uint16_t *) &r163DControl;
    //ESP_ERROR_CHECK(wm8960_writeReg(R16_3D_CONTROL_ADR, reg));



    // ADC VOLUME
    R21_LEFT_ADC_VOLUME_t leftAdcVolume = {
            .ADCVU = 1, // adc vol update
            /*
             *  Left ADC Digital Volume Control
                0000 0000 = Digital Mute
                0000 0001 = -97dB
                0000 0010 = -96.5dB
                ... 0.5dB steps up to
                1111 1111 = +30dB
             */
            .LADCVOL = 0xC3   // set to 0 db
    };
    reg = *(uint16_t *) &leftAdcVolume;
    ESP_ERROR_CHECK(wm8960_writeReg(R21_LEFT_ADC_VOLUME_ADR, reg));

    R22_RIGHT_ADC_VOLUME_t rightAdcVolume = {
            .ADCVU = 1,
            .RADCVOL = 0xC3
    };
    reg = *(uint16_t *) &rightAdcVolume;
    ESP_ERROR_CHECK(wm8960_writeReg(R22_RIGHT_ADC_VOLUME_ADR, reg));

    R23_ADDITIONAL_CONTROL_1_t additionalControl1 = {
            .TSDEN = 1, // thermal shutdown enabled
            .VSEL = 0b11, // analog bias 11 = default for 3.3v
            .DMONOMIX = 1, // 1= MONO,0=STEREO
            /*
             *  ADC Data Output Select
                00: left data = left ADC; right data =right ADC
                01: left data = left ADC; right data = left ADC
                10: left data = right ADC; right data =right ADC
                11: left data = right ADC; right data = left ADC
             */
            .DATSEL = 0b00,
            .TOCLKSEL =0, // page 74 volume upd timeout
            .TOEN = 0
    };
    reg = *(uint16_t *) &additionalControl1;
    ESP_ERROR_CHECK(wm8960_writeReg(R23_ADDITIONAL_CONTROL_1_ADR, reg));

    R24_ADDITIONAL_CONTROL_2_t r24AdditionalControl2 = {
            .HPSWEN = 0, // headph switch en
            .HPSWPOL = 0, // hph sw polarity 0= high = headph
            /*
             * Tristates ADCDAT and switches ADCLRC,
            DACLRC and BCLK to inputs.
            0 = ADCDAT is an output; ADCLRC, DACLRC
            and BCLK are inputs (slave mode) or outputs
            (master mode)
            1 = ADCDAT is tristated; DACLRC and BCLK
            are inputs; ADCLRC is an input (when not
            configured as a GPIO)
             */
            .TRIS =0,
            /*
             * Selects disable mode for ADCLRC and DACLRC
            (Master mode)
            0 = ADCLRC disabled when ADC (Left and
            Right) disabled; DACLRC disabled when
            DAC (Left and Right) disabled.
            1 = ADCLRC and DACLRC disabled only when
            ADC (Left and Right) and DAC (Left and Right)
            are disabled.
             */
            .LRCM=0
    };
    reg = *(uint16_t*)&r24AdditionalControl2;
    //   ESP_ERROR_CHECK(wm8960_writeReg(R24_ADDITIONAL_CONTROL_2_ADR, reg));

    R27_ADDITIONAL_CONTROL_3_t r27AdditionalControl3 = {
            .VROI = 0, // VREF to Analogue Output Resistance
            .OUT3CAP =0, // Capless Mode Headphone Switch Enable
            .ADC_ALC_SR = 0x00 // 0=44/48kHz 101=8k
    };
    reg = *(uint16_t*)&r27AdditionalControl3;
    //   ESP_ERROR_CHECK(wm8960_writeReg(R27_ADDITIONAL_CONTROL_3_ADR, reg));

    R28_ANTI_POP_1_t antiPop1 = {
            .POBCTRL = 0, //Selects the bias current source for output amplifiers and VMID buffer
            .BUFDCOPEN =1, //Enables the VGS / R current generator
            .BUFIOEN =1,
            .SOFT_ST =1, // vmid soft start
            .HPSTBY=0 // headphone standby
    };
    reg = *(uint16_t*)& antiPop1 ;
    //   ESP_ERROR_CHECK(wm8960_writeReg(R28_ANTI_POP_1_ADR, reg));

    R29_ANTI_POP_2_t antiPop2 = {
            .DISOP =0, // discharge headphone capacitors
            .DRES =0, // discharge resistor value
    };

    R34_LEFT_OUT_MIX_1_t leftOutMix1 = {
            .LD2LO = 1, // left dac to left out mixer
            .LI2LO = 0, // LINPUT3 to left out mixer
            .LI2LOVOL = 0b101 // linput3 to mix volume 000=0db 111=-21db
    };
    reg = *(uint16_t *) &leftOutMix1;
    ESP_ERROR_CHECK(wm8960_writeReg(R34_LEFT_OUT_MIX_2, reg));

    R37_RIGHT_OUT_MIX_2_t rightOutMix2 = {
            .RD2RO = 1, // dac to mix
            .RI2RO =0, // RINPUT3 disable
            .RI2ROVOL = 0b101
    };
    reg = *(uint16_t *) &rightOutMix2;
    ESP_ERROR_CHECK(wm8960_writeReg(R37_RIGHT_OUT_MIX_2, reg));

    R38_MONO_OUT_MIX_1_t r38MonoOutMix1 = {
            .L2MO =1 // connect left out mix to mono mixer
    };
    reg = *(uint16_t *) &r38MonoOutMix1;
    ESP_ERROR_CHECK(wm8960_writeReg(R38_MONO_OUT_MIX_1, reg));

    R39_MONO_OUT_MIX_2_t r39MonoOutMix2 = {
            .R2MO = 1
    };
    reg = *(uint16_t *) &r39MonoOutMix2;
    ESP_ERROR_CHECK(wm8960_writeReg(R39_MONO_OUT_MIX_2, reg));



    R42_MONOOUT_VOLUME_t r42MonooutVolume = {
            .MOUTVOL = 1 // mono out vol 1=-6db 0=0db
    };
    reg = *(uint16_t *) &r42MonooutVolume;
    ESP_ERROR_CHECK(wm8960_writeReg(R42_MONOOUT_VOLUME, reg));

    R43_INPUT_BOOST_MIXER_1_t r43InputBoostMixer1 = {
            .LIN2BOOST =0b000, // LINPUT2 to boost mixer volume  000 = mute 001=-12db 111=+6db
            .LIN3BOOST =0b000 // same with LINPUT3
    };
    reg = *(uint16_t *) &r43InputBoostMixer1;
    //ESP_ERROR_CHECK(wm8960_writeReg(R43_INPUT_BOOST_MIXER_1, reg));

    R44_INPUT_BOOST_MIXER_2_t r44InputBoostMixer2 = {
            .RIN2BOOST =0,
            .RIN3BOOST =0
    };
    reg = *(uint16_t *) &r44InputBoostMixer2;
    //ESP_ERROR_CHECK(wm8960_writeReg(R44_INPUT_BOOST_MIXER_2, reg));

    R45_BYPASS_1_t r45Bypass1 = {
            .LB2LO =0, // left boost mixer to out bypass
            .LB2LOVOL = 0b00 // bypass volume 0=0 111=-21db
    };
    reg = *(uint16_t *) &r45Bypass1;
    // ESP_ERROR_CHECK(wm8960_writeReg(R45_BYPASS_1, reg));

    R46_BYPASS_2_t r46Bypass2 = {
            .RB2RO = 0,
            .RB2ROVOL =0b00
    };
    reg = *(uint16_t *) &r46Bypass2;
    // ESP_ERROR_CHECK(wm8960_writeReg(R46_BYPASS_2, reg));

    //R48_ADDITIONAL_CONTROL_4_t //default

    R49_CLASS_D_CONTROL_1_t classDControl1 = {
            .SPK_OP_EN = 0b01 // enable both speakers 0b01=left only
    };
    reg = *(uint16_t *) &classDControl1;
    ESP_ERROR_CHECK(wm8960_writeReg(R49_CLASS_D_CTRL_1, reg));

    //R51_CLASS_D_CONTROL_3_t speaker boost settings default


}

