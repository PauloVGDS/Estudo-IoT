#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define I2C_MASTER_NUM I2C_NUM_0       // Porta I2C usada
#define I2C_MASTER_SDA_IO 19           // Pino SDA
#define I2C_MASTER_SCL_IO 18           // Pino SCL
#define I2C_MASTER_FREQ_HZ 100000      // Frequência do barramento I2C
#define MPU6050_ADDR 0x68              // Endereço I2C do MPU6050
#define MPU6050_PWR_MGMT_1 0x6B        // Registrador de gerenciamento de energia
#define MPU6050_ACCEL_XOUT_H 0x3B      // Registrador de dados do acelerômetro
#define MPU6050_ACCEL_CONFIG 0x1C      // Registrador usado para configurar a precisão do acelerômetro

static const char *TAG = "MPU6050";

void i2c_master_init() {
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

void mpu6050_write_register(uint8_t reg, uint8_t data) {
    uint8_t write_buf[2] = {reg, data};
    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_MASTER_NUM, MPU6050_ADDR, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS));
}

void mpu6050_read_register(uint8_t reg, uint8_t *data, size_t len) {
    ESP_ERROR_CHECK(i2c_master_write_read_device(I2C_MASTER_NUM, MPU6050_ADDR, &reg, 1, data, len, 1000 / portTICK_PERIOD_MS));
}

void mpu6050_init() {
    ESP_LOGI(TAG, "Inicializando MPU6050...");
    mpu6050_write_register(MPU6050_PWR_MGMT_1, 0x00); // Tira o sensor do modo de suspensão
    ESP_LOGI(TAG, "MPU6050 inicializado!");
}

void task_mpu6050(void *pvParameters) {
    uint8_t accel_data[6];
    int16_t accel_x, accel_y, accel_z;
    float accel_x_ms2, accel_y_ms2, accel_z_ms2;

    while (1) {
        // Lê os valores do acelerômetro
        
        mpu6050_read_register(MPU6050_ACCEL_XOUT_H, accel_data, 6);

        // Converte os valores brutos
        accel_x = (int16_t)((accel_data[0] << 8) | accel_data[1]);
        accel_x = (int16_t)((accel_data[0] << 8) | accel_data[1]);
        accel_y = (int16_t)((accel_data[2] << 8) | accel_data[3]);
        accel_z = (int16_t)((accel_data[4] << 8) | accel_data[5]);

        // 500 == 0000 0001(MSB) + 1111 0100(LSB)
        // 1 << 8 = 256 + 254 == 500
        // | = Operador Lógico OR
        // & = Operador Lógico AND


        accel_x_ms2 = (accel_x / 16384.0) * 9.8;
        accel_y_ms2 = (accel_y / 16384.0) * 9.8;
        accel_z_ms2 = (accel_z / 16384.0) * 9.8;

        ESP_LOGI(TAG, "Aceleração X: %.2f, Y: %.2f, Z: %.2f", accel_x_ms2, accel_y_ms2, accel_z_ms2);

        vTaskDelay(pdMS_TO_TICKS(1000)); // Aguarda 1 segundo

    }
}

void app_main() {
    i2c_master_init();
    mpu6050_init();
    mpu6050_write_register(MPU6050_ACCEL_CONFIG, 0x00);

    xTaskCreate(task_mpu6050, "MPU6050 Task", 4096, NULL, 5, NULL);
}