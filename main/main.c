#include "main.h"

#define TAG "main"
#define LV_TICK_PERIOD_MS 1

static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);
static void create_demo_application(void);
static void print_info(void);
void lv_example_list_1(void);

SemaphoreHandle_t xGuiSemaphore;
lv_obj_t *cw;

void app_main(void) {
    print_info();
    gpio_reset_pin(2);
    gpio_set_direction(2, GPIO_MODE_OUTPUT);
    xGuiSemaphore = xSemaphoreCreateMutex();
    xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 2, NULL, 2, NULL, 1);
    xTaskCreate(encoderTask,"encoderTask", 4096, NULL, 1, NULL);
    uint8_t i = 0;
    lv_obj_t *screen;


    while (1) {
        gpio_set_level(2, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));

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

        if (xSemaphoreTake(xGuiSemaphore, portMAX_DELAY) == pdTRUE) {
//       lv_obj_clear_flag(ui_startMenu, LV_OBJ_FLAG_HIDDEN);
//       lv_obj_clear_flag(ui_modeRoller, LV_OBJ_FLAG_HIDDEN);
//_ui_screen_change(ui_mixerMenuScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0);

            xSemaphoreGive(xGuiSemaphore);
        }
        gpio_set_level(2, 0);

        vTaskDelay(pdMS_TO_TICKS(1000));

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

    create_demo_application();
    _ui_screen_change(ui_mixerMenuScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0);
    xSemaphoreGive(xGuiSemaphore);
    for (;;) {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);


        }
    }
}

static void create_demo_application(void) {
    ui_init();

    //lv_example_list_1();

//    cw = lv_colorwheel_create(lv_scr_act(), true);
//    lv_obj_set_size(cw, 200, 200);
//    lv_obj_center(cw);
//    lv_colorwheel_set_rgb(cw ,lv_palette_main(LV_PALETTE_GREEN));

//    /*Create a chart*/
//    lv_obj_t * chart;
//    chart = lv_chart_create(lv_scr_act());
//    lv_obj_set_size(chart, 200, 150);
//    lv_obj_center(chart);
//    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/
//
//    /*Add two data series*/
//    lv_chart_series_t * ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
//    lv_chart_series_t * ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_SECONDARY_Y);
//
//    /*Set the next points on 'ser1'*/
//    lv_chart_set_next_value(chart, ser1, 10);
//    lv_chart_set_next_value(chart, ser1, 10);
//    lv_chart_set_next_value(chart, ser1, 10);
//    lv_chart_set_next_value(chart, ser1, 10);
//    lv_chart_set_next_value(chart, ser1, 10);
//    lv_chart_set_next_value(chart, ser1, 10);
//    lv_chart_set_next_value(chart, ser1, 10);
//    lv_chart_set_next_value(chart, ser1, 30);
//    lv_chart_set_next_value(chart, ser1, 70);
//    lv_chart_set_next_value(chart, ser1, 90);
//
//    /*Directly set points on 'ser2'*/
//    ser2->y_points[0] = 90;
//    ser2->y_points[1] = 70;
//    ser2->y_points[2] = 65;
//    ser2->y_points[3] = 65;
//    ser2->y_points[4] = 65;
//    ser2->y_points[5] = 65;
//    ser2->y_points[6] = 65;
//    ser2->y_points[7] = 65;
//    ser2->y_points[8] = 65;
//    ser2->y_points[9] = 65;
//
//    lv_chart_refresh(chart); /*Required after direct set*/
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

static lv_obj_t *list1;

static void event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked: %s", lv_list_get_btn_text(list1, obj));
    }
}

void lv_example_list_1(void) {
    /*Create a list*/
    list1 = lv_list_create(lv_scr_act());
    lv_obj_set_size(list1, 180, 220);
    lv_obj_center(list1);

    /*Add buttons to the list*/
    lv_obj_t *btn;

    lv_list_add_text(list1, "File");
    btn = lv_list_add_btn(list1, LV_SYMBOL_FILE, "New");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_DIRECTORY, "Open");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_SAVE, "Save");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_CLOSE, "Delete");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_EDIT, "Edit");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_list_add_text(list1, "Connectivity");
    btn = lv_list_add_btn(list1, LV_SYMBOL_BLUETOOTH, "Bluetooth");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_GPS, "Navigation");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_USB, "USB");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_BATTERY_FULL, "Battery");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_list_add_text(list1, "Exit");
    btn = lv_list_add_btn(list1, LV_SYMBOL_OK, "Apply");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_CLOSE, "Close");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
}