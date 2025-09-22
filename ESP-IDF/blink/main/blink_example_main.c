#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"



static const char *TAG = "LED";
static uint8_t s_led_state = 0;

static void blink_led(void)
{
    if (s_led_state == 1) {
        gpio_set_level(18, 1);
        gpio_set_level(19, 0);
    } else {
        gpio_set_level(19, 1);
        gpio_set_level(18, 0);
    }
    
    
    
    ESP_LOGI(TAG, "Estado do LED: %s", ((s_led_state == 1) ? "VERMELHO" : "AZUL"));
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Exemplo para piscar o LED do GPIO19!");
    gpio_reset_pin(18);
    gpio_reset_pin(19);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(18, GPIO_MODE_OUTPUT);
    gpio_set_direction(19, GPIO_MODE_OUTPUT);
}

void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    configure_led();

    while (1) {
        blink_led();
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
