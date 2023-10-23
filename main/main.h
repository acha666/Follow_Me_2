#ifndef __MAIN_H
#define __MAIN_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_freertos_hooks.h"
#include "esp_heap_caps.h"
#include "driver/spi_master.h"
#include "led_strip.h"

#define TFT_I2C_POWER_GPIO_PIN GPIO_NUM_21
#define WS2812_POWER_GPIO_PIN GPIO_NUM_34
#define WS2812_GPIO_PIN GPIO_NUM_33

extern SemaphoreHandle_t xGuiSemaphore;
extern led_strip_handle_t led_strip;

void vTaskLEDInit(void *arg);
void vTaskGUI(void *arg);
void vTaskLEDBlink(void *arg);

#endif