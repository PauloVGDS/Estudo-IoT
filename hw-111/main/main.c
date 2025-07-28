#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define I2C_MASTER_SCL_IO           22    // Pino SCL do I2C
#define I2C_MASTER_SDA_IO           21    // Pino SDA do I2C
#define I2C_MASTER_NUM              0     // Número do controlador I2C
#define I2C_MASTER_FREQ_HZ          400000 // Frequência do I2C: 400kHz
#define I2C_MASTER_TX_BUF_DISABLE   0     
#define I2C_MASTER_RX_BUF_DISABLE   0
#define DS1307_ADDR                 0x68  // Endereço I2C do DS1307

static const char *TAG = "HW-111_TEST";

// Estrutura para armazenar data/hora
typedef struct {
    uint8_t segundos;
    uint8_t minutos;
    uint8_t horas;
    uint8_t dia_semana;
    uint8_t dia;
    uint8_t mes;
    uint8_t ano;
} rtc_data_t;

// Função para converter BCD para decimal
static uint8_t bcd2dec(uint8_t val) {
    return (val >> 4) * 10 + (val & 0x0f);
}

// Função para converter decimal para BCD
static uint8_t dec2bcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

// Inicializa o I2C
static esp_err_t i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) return err;
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, 
                            I2C_MASTER_RX_BUF_DISABLE,
                            I2C_MASTER_TX_BUF_DISABLE, 0);
}

// Lê os dados do RTC
static esp_err_t rtc_get_data(rtc_data_t *data) {
    uint8_t rtc_reg[7];
    
    // Lê 7 bytes começando do registro 0x00
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (DS1307_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x00, true);  // Endereço inicial
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) return ret;

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (DS1307_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, rtc_reg, 7, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) return ret;

    // Converte os dados de BCD para decimal
    data->segundos = bcd2dec(rtc_reg[0] & 0x7F);
    data->minutos = bcd2dec(rtc_reg[1]);
    data->horas = bcd2dec(rtc_reg[2] & 0x3F);  // Formato 24h
    data->dia_semana = bcd2dec(rtc_reg[3]);
    data->dia = bcd2dec(rtc_reg[4]);
    data->mes = bcd2dec(rtc_reg[5]);
    data->ano = bcd2dec(rtc_reg[6]);

    return ESP_OK;
}

// Configura data/hora inicial
static esp_err_t rtc_set_initial_data(void) {
    uint8_t rtc_reg[8];
    
    // Define data inicial: 01/01/2024 00:00:00 Segunda-feira
    rtc_reg[0] = 0x00;  // Endereço inicial
    rtc_reg[1] = dec2bcd(0);     // Segundos
    rtc_reg[2] = dec2bcd(0);     // Minutos
    rtc_reg[3] = dec2bcd(0);     // Horas (formato 24h)
    rtc_reg[4] = dec2bcd(2);     // Dia da semana (1-7, 1=Domingo)
    rtc_reg[5] = dec2bcd(1);     // Dia
    rtc_reg[6] = dec2bcd(1);     // Mês
    rtc_reg[7] = dec2bcd(24);    // Ano

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (DS1307_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, rtc_reg, 8, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

void app_main(void)
{
    ESP_LOGI(TAG, "Iniciando teste do HW-111 (RTC DS1307)...");

    // Inicializa I2C
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C inicializado com sucesso");

    // Configura data/hora inicial
    ESP_ERROR_CHECK(rtc_set_initial_data());
    ESP_LOGI(TAG, "Data/hora inicial configurada");

    rtc_data_t rtc_data;
    char dias_semana[7][15] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};

    // Loop principal
    while (1) {
        esp_err_t ret = rtc_get_data(&rtc_data);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Data: %02d/%02d/20%02d", 
                    rtc_data.dia, rtc_data.mes, rtc_data.ano);
            ESP_LOGI(TAG, "Hora: %02d:%02d:%02d", 
                    rtc_data.horas, rtc_data.minutos, rtc_data.segundos);
            ESP_LOGI(TAG, "Dia da semana: %s", 
                    dias_semana[rtc_data.dia_semana - 1]);
        } else {
            ESP_LOGE(TAG, "Erro ao ler RTC: %d", ret);
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo
    }
}