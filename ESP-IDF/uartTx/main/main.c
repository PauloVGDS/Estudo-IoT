#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define UART_PORT UART_NUM_1
#define TX_PIN    GPIO_NUM_18
#define RX_PIN    GPIO_NUM_19

static const char *TAG = "UART";
static int count = 0;

void sendData(void *pvParameters);

void app_main(void)
{
    ESP_LOGI(TAG, "Configurando UART...");
    uart_config_t config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_PORT, &config);
    uart_set_pin(UART_PORT, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_PORT, 1024, 0, 0, NULL, 0);

    ESP_LOGI(TAG, "UART configurada com sucesso!");
    xTaskCreate(sendData, "Envio", 2048, NULL, 1, NULL);
}

void sendData(void *pvParameters) {
    char mensagem[] = "Especialistas em (a)creditar!\r\n\n";
    while (1) {
        mensagem[sizeof(mensagem) - 2] = count + '0'; // Atualiza o último caractere com o valor de count
        if (count > 9) {
            count = 0; // Reseta o contador se passar de 9
        }
        count++;
        uart_write_bytes(UART_PORT, mensagem, strlen(mensagem));
        ESP_LOGI(TAG, "Enviando: %s", mensagem);
        vTaskDelay(pdMS_TO_TICKS(2000)); // envia a cada 2s
    }
}
