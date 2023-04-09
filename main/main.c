#include "main.h"

#include "esp_chip_info.h"
#include "esp_flash.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "lvgl.h"
#include "st7789.h"
#include "encoder.h"
#include "button.h"
#include "ui_controls.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "driver/ledc.h"
#include "wm8960.h"
#include "sd_card.h"

#define TAG "main"
#define LV_TICK_PERIOD_MS (1)
#define ADC_SAMPLES 10
#define BATTERY_MAX_MV 4200
#define BATTERY_MIN_MV 3500

static void lv_tick_task(void *arg);
static void adc_init(void);
static uint32_t get_voltage_adc (void);
static int get_battery_charge(void);
static void pwm_init(void);

SemaphoreHandle_t xGuiSemaphore;
TaskHandle_t wav_task_handle = NULL;
TaskHandle_t i2s_task_handle = NULL;
TaskHandle_t main_task_handle = NULL;

void app_main(void) {

    mount_sdcard();

    xGuiSemaphore = xSemaphoreCreateMutex();
    lv_init();
    disp_spi_add_device(TFT_SPI_HOST);
    st7789_init();
    //buffer init
    lv_color_t *buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);
    static lv_color_t *buf2 = NULL;
    //lv_color_t *buf2 = heap_caps_malloc(4096 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    static lv_disp_draw_buf_t disp_buf;
    uint32_t size_in_px = DISP_BUF_SIZE;
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, size_in_px);
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = st7789_flush;
    disp_drv.rotated = 1;
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &lv_tick_task,
            .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
    ui_init();
    changeScreen(ui_startScreen);
    currentScreenUpd = ui_updateStartScreen;

    button_init();
    pcnt_unit_handle_t enc = encoderInit();
    adc_init();
    pwm_init();
    set_pwm_backlight(50);

    UBaseType_t stackWMark =0;
    TickType_t last_input_ack=0;
    TickType_t last_adc_ack=0;
    TickType_t last_gui_update=0;
    button_t* button_event = NULL;
    int encoder_delta=0;
    uint8_t noInputEventCnt =0;


    main_task_handle = xTaskGetCurrentTaskHandle();
    xTaskCreatePinnedToCore(i2s_task,"i2sTask", 4096, NULL, 1,&i2s_task_handle,0);
    xTaskCreatePinnedToCore(wav_task,"wavTask", 4096, NULL, 1,&wav_task_handle,1);



    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));

        // handle buttons and encoder
        if ((xTaskGetTickCount() - last_input_ack) > pdMS_TO_TICKS(INPUT_ACK_DELAY)) {
            button_event = get_button_event();
            encoder_delta = encoderGetPulseCount(enc);
            last_input_ack = xTaskGetTickCount();
            noInputEventCnt++; // if no input event GUI will be updated anyway every 4 iterations
        }

        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            // update GUI
            if (encoder_delta || button_event!= NULL || noInputEventCnt == 4) {
                (*currentScreenUpd)(encoder_delta,button_event);
                encoder_delta=0; //clear this values after
                button_event = NULL;
                noInputEventCnt=0;
            }

            // battery measurement
            if (((xTaskGetTickCount() - last_adc_ack) > pdMS_TO_TICKS(2000))) {
                uint8_t charge = get_battery_charge();
                last_adc_ack = xTaskGetTickCount();
                if (lv_scr_act() == ui_startScreen) {ui_set_battery_bar(charge);}
                //stackWMark = uxTaskGetStackHighWaterMark(xTaskGetHandle("main"));
                //printf(" watermark %d \n", stackWMark);
            }
        }
        xSemaphoreGive(xGuiSemaphore);


    }
}

static void lv_tick_task(void *arg) {
    (void) arg;
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

static esp_adc_cal_characteristics_t adc1_chars;
static void adc_init (void) {
    esp_err_t ret;
   // bool cali_enable = false;

    ret = esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF);
    if (ret == ESP_ERR_NOT_SUPPORTED) {
        ESP_LOGW(TAG, "Calibration scheme not supported, skip software calibration");
    } else if (ret == ESP_ERR_INVALID_VERSION) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else if (ret == ESP_OK) {
       // cali_enable = true;
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    } else {
        ESP_LOGE(TAG, "Invalid arg");
    }

    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11));

}
static uint32_t get_voltage_adc (void) {
    uint32_t voltage=0;
    static int adc_raw [ADC_SAMPLES] = {};
    static uint8_t count =0;
    static bool samples_ready=0;
    
    adc_raw[count] = adc1_get_raw(ADC1_CHANNEL_0);
    //ESP_LOGI(TAG, "raw  data: %d", adc_raw[count]);
    count++;
    
    if (count > ADC_SAMPLES-1) {count=0; samples_ready=1;}

    if (samples_ready) {
        for (uint8_t i = 0; i < ADC_SAMPLES; i++) {
            voltage += esp_adc_cal_raw_to_voltage(adc_raw[i], &adc1_chars);
        }
        voltage /= ADC_SAMPLES;
    } else {
        voltage = esp_adc_cal_raw_to_voltage(adc_raw[count], &adc1_chars);
    }

    //ESP_LOGI(TAG, "cali data: %d mV", voltage);
    return voltage;
}
static int get_battery_charge(void){
    uint32_t voltage = get_voltage_adc();
    voltage*=2;
    if (voltage > BATTERY_MIN_MV)
        voltage -= BATTERY_MIN_MV;
    else voltage = 0;

    float percent = (voltage*100.0f/(BATTERY_MAX_MV-BATTERY_MIN_MV));
    //printf ("PERCENT %f", percent);
    if (percent > 100) return 0xFF;
    return (int)percent;
}

static void pwm_init(void){
    ledc_timer_config_t ledc_timer = {
            .speed_mode       = LEDC_LOW_SPEED_MODE,
            .timer_num        = LEDC_TIMER_0,
            .duty_resolution  = LEDC_TIMER_13_BIT,
            .freq_hz          = 5000,  // Set output frequency at 5 kHz
            .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
            .speed_mode     = LEDC_LOW_SPEED_MODE,
            .channel        = LEDC_CHANNEL_0,
            .timer_sel      = LEDC_TIMER_0,
            .intr_type      = LEDC_INTR_DISABLE,
            .gpio_num       = 27,
            .duty           = 0, // Set duty to 0%
            .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}
void set_pwm_backlight (uint8_t percent){
    uint32_t duty = (8191/100)*percent;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
}

void set_vu_slider (int val){
    lv_slider_set_value(ui_mixer_vu_bar_slider, val, LV_ANIM_OFF);
}


/*void print_info(void) {
    printf("Hello world!\n");
    *//* Print chip information *//*
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%uMB %s flash\n", flash_size / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

//    printf("Restarting now.\n");
//    fflush(stdout);
//    esp_restart();
}*/




