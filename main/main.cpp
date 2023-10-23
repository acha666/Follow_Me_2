#include <main.h>

extern "C" void app_main(void)
{
    xTaskCreatePinnedToCore(vTaskLEDInit,"LED Init Task",1024*3,NULL,1,NULL,0);
    xTaskCreatePinnedToCore(vTaskGUI, "GUI Task", 1024 * 3, NULL, 1, NULL, 0);

}
