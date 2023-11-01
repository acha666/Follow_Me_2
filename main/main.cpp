#include <main.h>
#include "nvs_flash.h"

extern "C" void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // xTaskCreatePinnedToCore(vTaskLEDInit,"LED Init Task",1024*3,NULL,1,NULL,0);
    // xTaskCreatePinnedToCore(vTaskGUI, "GUI Task", 1024 * 3, NULL, 1, NULL, 0);
    // vTaskDelay(1000/portTICK_PERIOD_MS);
    // xTaskCreatePinnedToCore(vTaskI2C, "I2C Task", 1024 * 3, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(vTaskWifiScan, "WiFi Scan Task", 1024 * 3, NULL, 1, NULL, 0);
}
