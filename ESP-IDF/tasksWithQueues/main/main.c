#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"

QueueHandle_t fila;
static const char *FUNCTION = "MAIN";
static const char *TAG_SENDER = "SENDER";
static const char *TAG_RECEIVER = "RECEIVER";

void sender_task(void *pvParameters);
void receiver_task(void *pvParameters);


void app_main(void) {
    fila = xQueueCreate(5, sizeof(int));
    if (fila == NULL) {
        ESP_LOGI(FUNCTION, "Erro ao criar fila!");
        return;
    }
    
    xTaskCreate(sender_task, "Task de Envio", 2048, NULL, 1, NULL);
    xTaskCreate(receiver_task, "Task de Recebimento", 2048, NULL, 1, NULL);
}

void sender_task(void *pvParameters) {
    int count = 0;
    while (1)
    {
        count++;
        xQueueSend(fila, &count, portMAX_DELAY);
        ESP_LOGI(TAG_SENDER, "Valor adicionado na fila!");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}

void receiver_task(void *pvParameters) {
    int currentCountValue;
    while (1) {
        if (xQueueReceive(fila, &currentCountValue, portMAX_DELAY)) {
            ESP_LOGI(TAG_RECEIVER, "Valor recebido: %i", currentCountValue);
        }
    }

}