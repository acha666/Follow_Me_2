#include "main.h"
#include "led_strip.h"
#include "driver/gpio.h"

led_strip_handle_t led_strip;

void vTaskLEDInit(void *arg)
{
    /*Pull up WS2812_POWER pin*/
    gpio_set_direction(WS2812_POWER_GPIO_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(WS2812_POWER_GPIO_PIN, 1);

    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config;
    strip_config.strip_gpio_num = WS2812_GPIO_PIN;        // The GPIO that connected to the LED strip's data line
    strip_config.max_leds = 1;                            // The number of LEDs in the strip,
    strip_config.led_pixel_format = LED_PIXEL_FORMAT_GRB; // Pixel format of your LED strip
    strip_config.led_model = LED_MODEL_WS2812;            // LED strip model
    strip_config.flags.invert_out = false;                // whether to invert the output signal (useful when your hardware has a level inverter)

    led_strip_rmt_config_t rmt_config;
    rmt_config.clk_src = RMT_CLK_SRC_DEFAULT;    // different clock source can lead to different power consumption
    rmt_config.resolution_hz = 10 * 1000 * 1000; // 10MHz
    rmt_config.flags.with_dma = false;           // whether to enable the DMA feature

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));

    led_strip_clear(led_strip);

    xTaskCreatePinnedToCore(vTaskLEDBlink, "LED Blink Task", 1024 * 3, NULL, 1, NULL, 0);
    vTaskDelete(NULL);
}

void vTaskLEDBlink(void *arg)
{
    uint8_t led_stat = 0;
    while (1)
    {
        if (led_stat)
            led_strip_set_pixel(led_strip, 0, 64, 64, 64);
        else
            led_strip_clear(led_strip);

        led_stat = !led_stat;
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}