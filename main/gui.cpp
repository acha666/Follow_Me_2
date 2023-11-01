#include "main.h"
#include "driver/gpio.h"
#include "lvgl.h"
#include "lvgl_helpers.h"
#include "ui.h"

SemaphoreHandle_t xGuiSemaphore;
void lv_tick_task(void *arg)
{
    (void)arg;
    lv_tick_inc(portTICK_PERIOD_MS);
}

void vTaskGUI(void *arg)
{
    /*Pull up TFT_I2C_POWER pin*/
    gpio_set_direction(TFT_I2C_POWER_GPIO_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(TFT_I2C_POWER_GPIO_PIN, 1);

    xGuiSemaphore = xSemaphoreCreateMutex();
    lv_init();
    lvgl_driver_init();

    static lv_disp_draw_buf_t draw_buf;
    lv_color_t *buf1 = reinterpret_cast<lv_color_t *>(heap_caps_malloc(LV_HOR_RES_MAX / 2 * LV_VER_RES_MAX * sizeof(lv_color_t), MALLOC_CAP_DMA));
    lv_color_t *buf2 = reinterpret_cast<lv_color_t *>(heap_caps_malloc(LV_HOR_RES_MAX / 2 * LV_VER_RES_MAX * sizeof(lv_color_t), MALLOC_CAP_DMA));
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LV_HOR_RES_MAX / 2 * LV_VER_RES_MAX); /*Initialize the display buffer*/

    static lv_disp_drv_t disp_drv;         /*A variable to hold the drivers. Must be static or global.*/
    lv_disp_drv_init(&disp_drv);           /*Basic initialization*/
    disp_drv.draw_buf = &draw_buf;         /*Set an initialized buffer*/
    disp_drv.flush_cb = disp_driver_flush; /*Set a flush callback to draw to the display*/
    disp_drv.hor_res = LV_HOR_RES_MAX;     /*Set the horizontal resolution in pixels*/
    disp_drv.ver_res = LV_VER_RES_MAX;     /*Set the vertical resolution in pixels*/
    lv_disp_drv_register(&disp_drv);       /*Register the driver and save the created display objects*/
    esp_register_freertos_tick_hook(reinterpret_cast<esp_freertos_tick_cb_t>(lv_tick_task));

    ui_init();
    lv_label_set_text(ui_status,"Disconnected");

    while (1)
    {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            lv_timer_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
    }
}