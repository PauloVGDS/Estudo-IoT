#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"

#define LED_GPIO       GPIO_NUM_2
#define UART_PORT_NUM  UART_NUM_0
#define BUF_SIZE       128

static const char *TAG = "UART_COMMAND";

void uart_command_task(void *pvParameters) {
    uint8_t data;
    char buffer[BUF_SIZE] = {0};
    int pos = 0;

    while (1) {
        int len = uart_read_bytes(UART_PORT_NUM, &data, 1, portMAX_DELAY);
        if (len > 0) {
            // Echo do que o usuário digita
            uart_write_bytes(UART_PORT_NUM, (const char *)&data, 1);

            if (data == '\n' || data == '\r') {
                buffer[pos] = '\0';  // Finaliza a string

                ESP_LOGI(TAG, "Comando recebido: %s", buffer);
                if (strcmp(buffer, "ligar") == 0) {
                    gpio_set_level(LED_GPIO, 1);
                    uart_write_bytes(UART_PORT_NUM, "LED ligado!\n", 12);
                } else if (strcmp(buffer, "desligar") == 0) {
                    gpio_set_level(LED_GPIO, 0);
                    uart_write_bytes(UART_PORT_NUM, "LED desligado!\n", 15);
                } else {
                    uart_write_bytes(UART_PORT_NUM, "Comando inválido\n", 18);
                }

                pos = 0;  // Reinicia buffer
                memset(buffer, 0, BUF_SIZE);
            } else if (pos < BUF_SIZE - 1) {
                buffer[pos++] = data;
            }
        }
    }
}

void app_main(void) {
    // Configura o LED
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << LED_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    // Inicializa UART0 (já é o terminal padrão do monitor)
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_PORT_NUM, &uart_config);
    uart_driver_install(UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);

    // Cria a task de comandos
    xTaskCreate(uart_command_task, "uart_command_task", 4096, NULL, 5, NULL);
}
