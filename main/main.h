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
#include "driver/i2c.h"
#include "led_strip.h"

constexpr gpio_num_t TFT_I2C_POWER_GPIO_PIN = GPIO_NUM_21;
constexpr gpio_num_t WS2812_POWER_GPIO_PIN = GPIO_NUM_34;
constexpr gpio_num_t BUTTON_GPIO_PIN = GPIO_NUM_0;
constexpr gpio_num_t WS2812_GPIO_PIN = GPIO_NUM_33;

constexpr i2c_port_t I2C_PORT = I2C_NUM_0;
constexpr uint8_t I2C_SCL_PIN = 41;
constexpr uint8_t I2C_SDA_PIN = 42;
constexpr uint32_t I2C_MASTER_FREQ = 100000;

#define WIFI_SELECT_1 1
#if WIFI_SELECT_1
constexpr char* WIFI_SSID = "MeiYouMiMa_2.4G";
constexpr char* WIFI_PASSWORD = "MeiYouMiMa456";
#elif WIFI_SELECT_2
constexpr char* WIFI_SSID = "7Makers_5G"; 
constexpr char* WIFI_PASSWORD = "12345679";
#endif

extern SemaphoreHandle_t xGuiSemaphore;
extern led_strip_handle_t led_strip;
extern xQueueHandle gpioEventQueue;

extern uint8_t led_stat;

static void IRAM_ATTR gpioIntrHandler(void* arg);

void vTaskLEDInit(void *arg);
void vTaskLEDBlink(void *arg);
void vTaskGUI(void *arg);
void vTaskI2C(void *arg);
void vTaskWifi(void *arg);
void vTaskGPIO(void *arg)

#endif