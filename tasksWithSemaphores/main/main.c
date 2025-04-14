#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "driver/gpio.h"


SemaphoreHandle_t xSemaphore;

void task1(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY)) {
            printf("Task 1 acessando recurso compartilhado\n");
            vTaskDelay(pdMS_TO_TICKS(500)); // Simula o uso do recurso
            xSemaphoreGive(xSemaphore);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}

void task2(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY)) {
            printf("Task 2 acessando recurso compartilhado\n");
            vTaskDelay(pdMS_TO_TICKS(500)); // Simula o uso do recurso
            xSemaphoreGive(xSemaphore);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}

void app_main(void) {
    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore); // Inicializa o semáforo como disponível

    xTaskCreate(task1, "Task 1", 2048, NULL, 1, NULL);
    xTaskCreate(task2, "Task 2", 2048, NULL, 1, NULL);
}