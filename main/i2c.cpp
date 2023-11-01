#include "main.h"
#include "driver/i2c.h"

#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define WRITE_BIT I2C_MASTER_WRITE  /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ    /*!< I2C master read */
#define ACK_CHECK_EN 0x1            /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0           /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                 /*!< I2C ack value */
#define NACK_VAL 0x1                /*!< I2C nack value */

void vTaskI2C(void *arg)
{
    constexpr char *TAG = "I2C Task";

    // Pull up TFT_I2C_POWER pin
    // Done by vTaskGUI()
    // gpio_set_direction(TFT_I2C_POWER_GPIO_PIN, GPIO_MODE_OUTPUT);
    // gpio_set_level(TFT_I2C_POWER_GPIO_PIN, 1);

    i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_SDA_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_SCL_PIN;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ;
    // conf.clk_flags = 0;          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */

    i2c_param_config(I2C_PORT, &conf);
    uint8_t address;
    ESP_LOGI(TAG, "     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
    for (int i = 0; i < 128; i += 16)
    {
        ESP_LOGI(TAG, "%02x: ", i);
        for (int j = 0; j < 16; j++)
        {
            address = i + j;
            i2c_cmd_handle_t cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (address << 1) | WRITE_BIT, ACK_CHECK_EN);
            i2c_master_stop(cmd);
            esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 50 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd);
            if (ret == ESP_OK)
            {
                ESP_LOGI(TAG, "%02x ", address);
            }
            else if (ret == ESP_ERR_TIMEOUT)
            {
                ESP_LOGI(TAG, "UU ");
            }
            else
            {
                ESP_LOGI(TAG, "-- ");
            }
        }
        ESP_LOGI(TAG, "\r\n");
    }

    i2c_driver_delete(I2C_PORT);
    vTaskDelete(NULL);
}