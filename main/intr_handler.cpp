#include "main.h"

xQueueHandle gpioEventQueue=NULL;
static void IRAM_ATTR gpioIntrHandler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpioEventQueue, &gpio_num, NULL);
}