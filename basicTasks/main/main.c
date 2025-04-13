#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

// Constantes
#define LED GPIO_NUM_19
#define SAIDA GPIO_MODE_OUTPUT
#define LIGADO 1
#define DESLIGADO 0
static const char *TAG = "INFO";

// Protótipos
void blink(void *pvParameters);
void message(void *pvParameters);


// Função Principal
void app_main(void)
{
    // Definição do Pino como saída
    gpio_set_direction(LED, SAIDA);

    // Criação das Tasks
    xTaskCreate(blink, "Blink LED", 2048, NULL, 3, NULL);
    xTaskCreate(message, "Mensagem", 2048, NULL, 1, NULL);


}


// Task 1
void blink(void *pvParameters) {
    while (1)
    {
        // Nível Alto
        gpio_set_level(LED, LIGADO);
        // Bloqueia a task por 500ms
        vTaskDelay(pdMS_TO_TICKS(500));
        // Nível Baixo
        gpio_set_level(LED, DESLIGADO);
        // Bloqueia a task por 500ms
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
}

// Task 2
void message(void *pvParameters) {
    while (1)
    {
        // Log da mensagem
        ESP_LOGI(TAG, "Hello World!");
        // Bloqueia a Task por 500ms
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
}