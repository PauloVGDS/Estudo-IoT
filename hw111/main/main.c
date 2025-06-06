#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <time.h>

#define I2C_MASTER_NUM      I2C_NUM_0
#define I2C_MASTER_SDA_IO   19
#define I2C_MASTER_SCL_IO   18
#define I2C_MASTER_FREQ_HZ  100000
#define DS3231_ADDR         0x68

static const char *TAG = "HW111_RTC";

static uint8_t bcd_to_decimal(uint8_t val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}

static void i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0));
}

static esp_err_t ds3231_read_time(struct tm *timeinfo)
{
    uint8_t reg = 0x00;
    uint8_t data[7];
    esp_err_t ret = i2c_master_write_read_device(I2C_MASTER_NUM, DS3231_ADDR,
                                                 &reg, 1, data, sizeof(data),
                                                 1000 / portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
        return ret;
    }

    timeinfo->tm_sec  = bcd_to_decimal(data[0]);
    timeinfo->tm_min  = bcd_to_decimal(data[1]);
    timeinfo->tm_hour = bcd_to_decimal(data[2]);
    timeinfo->tm_mday = bcd_to_decimal(data[4]);
    timeinfo->tm_mon  = bcd_to_decimal(data[5] & 0x1F) - 1;
    timeinfo->tm_year = bcd_to_decimal(data[6]) + 100; // year since 1900

    return ESP_OK;
}

void app_main(void)
{
    i2c_master_init();

    struct tm rtc_time;
    while (1) {
        if (ds3231_read_time(&rtc_time) == ESP_OK) {
            ESP_LOGI(TAG,
                     "%02d/%02d/%04d %02d:%02d:%02d",
                     rtc_time.tm_mday,
                     rtc_time.tm_mon + 1,
                     rtc_time.tm_year + 1900,
                     rtc_time.tm_hour,
                     rtc_time.tm_min,
                     rtc_time.tm_sec);
        } else {
            ESP_LOGE(TAG, "Erro ao ler o RTC");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

