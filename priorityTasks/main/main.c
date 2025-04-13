#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Constantes
#define LED_VERMELHO GPIO_NUM_18
#define LED_VERDE GPIO_NUM_19
#define LED_AZUL GPIO_NUM_21
#define LIGADO 1
#define DESLIGADO 0
#define SAIDA GPIO_MODE_OUTPUT
#define ENTRADA GPIO_MODE_INPUT

// Tags
static const char *TAG = "MAIN";

// Protótipos
void led_vermelho(void *pvParameters);
void led_azul(void *pvParameters);
void led_verde(void *pvParameters);


// Função Principal
void app_main(void) {
    ESP_LOGI(TAG, "Hello World!");

    gpio_set_direction(LED_VERMELHO, SAIDA);
    gpio_set_direction(LED_VERDE, SAIDA);
    gpio_set_direction(LED_AZUL, SAIDA);

    xTaskCreate(led_vermelho, "VERMELHO", 1024, NULL, 3, NULL);
    xTaskCreate(led_azul, "AZUL", 1024, NULL, 2, NULL);
    xTaskCreate(led_verde, "VERDE", 1024, NULL, 1, NULL);

}

// Tasks
void led_vermelho(void *pvParameters) {
    while (1) {
        gpio_set_level(LED_VERMELHO, LIGADO);
        vTaskDelay(250 / portTICK_PERIOD_MS);
        gpio_set_level(LED_VERDE, DESLIGADO);    
        gpio_set_level(LED_AZUL, DESLIGADO);  

    }
}

void led_azul(void *pvParameters) {
    while (1) {
        gpio_set_level(LED_AZUL, LIGADO);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_level(LED_VERDE, DESLIGADO);    
        gpio_set_level(LED_VERMELHO, DESLIGADO);
    }
}

void led_verde(void *pvParameters) {
    while (1) {
        gpio_set_level(LED_VERDE, LIGADO);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(LED_AZUL, DESLIGADO);    
        gpio_set_level(LED_VERMELHO, DESLIGADO);    
    }
}
