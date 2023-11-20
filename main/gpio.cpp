#include "main.h"

void vTaskGPIO(void *arg)
{
    uint32_t io_num;
    for (;;)
    {
        if (xQueueReceive(gpioEventQueue, &io_num, portMAX_DELAY))
        {
            //switch led on/off
            if (io_num == BUTTON_GPIO_PIN)
            {
                if (led_stat == 0)
                {
                    led_stat = 1;
                    led_strip->set_pixel(led_strip, 0, 0, 0, 255);
                    led_strip->refresh(led_strip, 100);
                }
                else
                {
                    led_stat = 0;
                    led_strip->set_pixel(led_strip, 0, 0, 0, 0);
                    led_strip->refresh(led_strip, 100);
                }
            }
        }
    }
}