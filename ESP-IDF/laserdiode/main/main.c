#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "lora_config.h"
#include "lora_registers.h"

// Define o modo de operação: 1 para TX, 0 para RX
#define MODO_TRANSMISSOR 1

static const char *TAG = "LoRa";

// Handle do dispositivo SPI
spi_device_handle_t spi;

// Funções auxiliares para comunicação SPI
void lora_write_reg(uint8_t reg, uint8_t value) {
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    
    uint8_t out[2] = {reg | 0x80, value};
    t.length = 16;
    t.tx_buffer = out;
    t.rx_buffer = NULL;
    
    gpio_set_level(LORA_CS_PIN, 0);
    ret = spi_device_transmit(spi, &t);
    gpio_set_level(LORA_CS_PIN, 1);
    
    ESP_ERROR_CHECK(ret);
}

uint8_t lora_read_reg(uint8_t reg) {
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    
    uint8_t out[2] = {reg & 0x7F, 0xFF};
    uint8_t in[2];
    t.length = 16;
    t.tx_buffer = out;
    t.rx_buffer = in;
    
    gpio_set_level(LORA_CS_PIN, 0);
    ret = spi_device_transmit(spi, &t);
    gpio_set_level(LORA_CS_PIN, 1);
    
    ESP_ERROR_CHECK(ret);
    return in[1];
}

void lora_init() {
    // Reset do módulo
    gpio_set_level(LORA_RST_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(1));
    gpio_set_level(LORA_RST_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    // Configura modo LoRa
    lora_write_reg(REG_OP_MODE, MODE_SLEEP);
    lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
    
    // Configura frequência (915MHz)
    uint64_t frf = ((uint64_t)LORA_FREQUENCY << 19) / 32000000;
    lora_write_reg(REG_FR_MSB, (uint8_t)(frf >> 16));
    lora_write_reg(REG_FR_MID, (uint8_t)(frf >> 8));
    lora_write_reg(REG_FR_LSB, (uint8_t)(frf >> 0));
    
    // Configura potência de transmissão
    lora_write_reg(REG_PA_CONFIG, 0x8F);  // Max power
    
    // Configura LNA
    lora_write_reg(REG_LNA, 0x23);  // Boost on, gain máximo
    
    // Configura modem
    lora_write_reg(REG_MODEM_CONFIG_1, 
        (0x07 << 4) |  // Bandwidth 125kHz
        (0x01 << 1) |  // Coding rate 4/5
        0x00           // Explicit header mode
    );
    
    lora_write_reg(REG_MODEM_CONFIG_2,
        (LORA_SPREADING_FACTOR << 4) |  // Spreading factor
        (0x01 << 2)                     // CRC enabled
    );
    
    // Configura palavra de sincronização
    lora_write_reg(REG_SYNC_WORD, 0x12);
    
    // Coloca em modo standby
    lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
    
    ESP_LOGI(TAG, "LoRa inicializado com sucesso!");
    ESP_LOGI(TAG, "Modo: %s", MODO_TRANSMISSOR ? "Transmissor" : "Receptor");
}

void lora_send_data(const char* data) {
    int len = strlen(data);
    
    // Configura modo TX
    lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
    lora_write_reg(REG_FIFO_ADDR_PTR, 0);
    
    // Escreve dados no FIFO
    for(int i = 0; i < len; i++) {
        lora_write_reg(REG_FIFO, data[i]);
    }
    
    // Define tamanho do payload
    lora_write_reg(REG_PAYLOAD_LENGTH, len);
    
    // Inicia transmissão
    lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);
    
    // Aguarda fim da transmissão
    while((lora_read_reg(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    // Limpa flag de TX
    lora_write_reg(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    
    ESP_LOGI(TAG, "Dados enviados: %s", data);
}

void lora_receive_data(void) {
    // Configura modo RX contínuo
    lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
    
    // Aguarda recebimento de dados
    while((lora_read_reg(REG_IRQ_FLAGS) & IRQ_RX_DONE_MASK) == 0) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    // Verifica se há erro de CRC
    if(lora_read_reg(REG_IRQ_FLAGS) & IRQ_PAYLOAD_CRC_ERROR_MASK) {
        ESP_LOGE(TAG, "Erro de CRC!");
        lora_write_reg(REG_IRQ_FLAGS, IRQ_PAYLOAD_CRC_ERROR_MASK);
        return;
    }
    
    // Lê o número de bytes recebidos
    int len = lora_read_reg(REG_RX_NB_BYTES);
    
    // Move ponteiro do FIFO para início dos dados
    lora_write_reg(REG_FIFO_ADDR_PTR, lora_read_reg(REG_FIFO_RX_CURRENT));
    
    // Lê os dados
    char data[256];
    for(int i = 0; i < len; i++) {
        data[i] = lora_read_reg(REG_FIFO);
    }
    data[len] = '\0';
    
    // Limpa flag de RX
    lora_write_reg(REG_IRQ_FLAGS, IRQ_RX_DONE_MASK);
    
    // Calcula RSSI
    int rssi = lora_read_reg(REG_PKT_RSSI_VALUE) - 137;
    
    ESP_LOGI(TAG, "Dados recebidos: %s (RSSI: %d)", data, rssi);
}

void spi_init(void) {
    esp_err_t ret;
    
    spi_bus_config_t buscfg = {
        .miso_io_num = LORA_MISO_PIN,
        .mosi_io_num = LORA_MOSI_PIN,
        .sclk_io_num = LORA_SCK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 9000000,
        .mode = 0,
        .spics_io_num = LORA_CS_PIN,
        .queue_size = 7,
    };

    ret = spi_bus_initialize(HSPI_HOST, &buscfg, 0);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
}

void lora_task(void *pvParameters) {
    if (MODO_TRANSMISSOR) {
        char data[] = "Teste LoRa ESP32";
        while (1) {
            lora_send_data(data);
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    } else {
        while (1) {
            lora_receive_data();
        }
    }
}

void app_main(void) {
    // Inicializa GPIO
    gpio_reset_pin(LORA_RST_PIN);
    gpio_set_direction(LORA_RST_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(LORA_CS_PIN);
    gpio_set_direction(LORA_CS_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LORA_CS_PIN, 1);

    // Inicializa SPI
    spi_init();
    
    // Inicializa LoRa
    lora_init();

    // Cria a task de acordo com o modo selecionado
    xTaskCreate(lora_task, "lora_task", 2048, NULL, 5, NULL);
}
