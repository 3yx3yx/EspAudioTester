#include "main.h"

#define TAG "main"
#define LV_TICK_PERIOD_MS 1

static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);
static void print_info(void);
static void changeScreen (lv_obj_t* screen);

SemaphoreHandle_t xGuiSemaphore;
pcnt_unit_handle_t enc;

void app_main(void) {
    print_info();
    enc = encoderInit();
    xGuiSemaphore = xSemaphoreCreateMutex();
    xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 2, NULL, 2, NULL, 1);


    while (1) {

/*        switch (i) {
            case 0:
                screen = ui_startScreen;
                break;
            case 1:
                screen = ui_mixerMenuScreen;
                break;
            case 2:
                screen = ui_recPlayScreen;
                break;
            case 3:
                screen = ui_tracklistScreen;
                break;
            case 4:
                screen = ui_acceptDeclineScreen;
                break;
            case 5:
                screen = ui_optionsScreen;
                break;
            case 6:
                screen = ui_cableTestScreen;
                break;
            default:
                screen = ui_startScreen;
                break;
        }
        i++;
        if (i > 6) i = 0;*/
        vTaskDelay(pdMS_TO_TICKS(100));


        int pulseCnt = encoderGetPulseCount(enc);
        if (pulseCnt)
        {
            printf ("pcnt %d \n", pulseCnt);
            if (xSemaphoreTake(xGuiSemaphore, portMAX_DELAY) == pdTRUE) {
                if (lv_disp_get_scr_act(NULL) == ui_startScreen) {
                    uint16_t current = lv_roller_get_selected(ui_modeRoller);
                    uint16_t max = lv_roller_get_option_cnt(ui_modeRoller);

                    lv_roller_set_selected(ui_modeRoller,
                                           encoderCalculatePosition(pulseCnt, current, 0, max),
                                           LV_ANIM_OFF);

                }
                xSemaphoreGive(xGuiSemaphore);
            }
        }



    }

}


static void guiTask(void *pvParameter) {
    (void) pvParameter; //unused
    xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
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
    //changeScreen(ui_mixerMenuScreen);
    changeScreen(ui_startScreen);
    xSemaphoreGive(xGuiSemaphore);

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(10));
        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
    }
}

static void changeScreen (lv_obj_t* screen){
    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, false);
}

static void lv_tick_task(void *arg) {
    (void) arg;
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

void print_info(void) {
    printf("Hello world!\n");
    /* Print chip information */
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
}




