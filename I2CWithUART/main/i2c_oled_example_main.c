#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lvgl_port.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"


#define I2C_BUS_PORT                0       // Configurações específicas para seu display SSD1306
#define I2C_SDA_PIN                 13
#define I2C_SCL_PIN                 12
#define I2C_ADDR                    0x3C
#define I2C_FREQ_HZ                 (400000)
#define LCD_WIDTH_PIXELS           128      // Tamanho do display
#define LCD_HEIGHT_PIXELS          64
#define LCD_CMD_BITS               8        // Parâmetros do display
#define LCD_PARAM_BITS             8
#define DC_BIT_OFFSET              6

#define UART_PORT UART_NUM_1
#define TX_PIN GPIO_NUM_10
#define RX_PIN GPIO_NUM_11



// Protótipos
lv_disp_t *configDisplay(void *pvParameters);
lv_obj_t *configUserInterface(lv_disp_t *disp);
void showUserInterface(void *pvParameters);
void updateUserInterface(lv_obj_t *label, char *text);
void configUart(void *pvParameters);

// Estáticos
static const char *TAG_I2C = "I2C";
static const char *TAG_UART = "UART";
static const char *TAG_LVGL = "LVGL";

static SemaphoreHandle_t lvgl_mutex;

void app_main(void)
{
    ESP_LOGI(TAG_I2C, "Configurando barramento I2C...");
    lv_disp_t *display = configDisplay(NULL);
    lv_obj_t *interface = configUserInterface(display);

    configUart(NULL);
    ESP_LOGI(TAG_UART, "UART configurado com sucesso!");

    lvgl_mutex = xSemaphoreCreateMutex();
    if (lvgl_mutex == NULL) {
        ESP_LOGE(TAG_I2C, "Falha ao criar mutex LVGL!");
        return;
    }

    xTaskCreate(showUserInterface, "Interface", 4096, interface, 5, NULL);
    
}


lv_disp_t *configDisplay(void *pvParameters) {
    ESP_LOGI(TAG_I2C, "Inicializando barramento I2C...");
    i2c_master_bus_handle_t i2c_bus = NULL;

    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .i2c_port = I2C_BUS_PORT,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));

    ESP_LOGI(TAG_I2C, "Criando canal de comunicação com o display...");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = I2C_ADDR,
        .scl_speed_hz = I2C_FREQ_HZ,
        .control_phase_bytes = 1,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .dc_bit_offset = DC_BIT_OFFSET,
    };
    
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));

    ESP_LOGI(TAG_I2C, "Inicializando driver do painel SSD1306...");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = LCD_HEIGHT_PIXELS,
    };
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = -1,
        .bits_per_pixel = 1,
        .vendor_config = &ssd1306_config,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    ESP_LOGI(TAG_LVGL, "Inicializando LVGL...");
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_port_init(&lvgl_cfg);

    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = LCD_WIDTH_PIXELS * LCD_HEIGHT_PIXELS,
        .double_buffer = true,
        .hres = LCD_WIDTH_PIXELS,
        .vres = LCD_HEIGHT_PIXELS,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }
    };
    lv_disp_t *disp = lvgl_port_add_disp(&disp_cfg);
    lv_disp_set_rotation(disp, LV_DISP_ROT_NONE);

    ESP_LOGI(TAG_LVGL, "Interface LVGL configurada!");
    return disp;
}

lv_obj_t *configUserInterface(lv_disp_t *disp) {
    // Cria um label
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(label, 100); 
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    ESP_LOGI(TAG_LVGL, "Label criado com sucesso!");
    return label;
}

void showUserInterface(void *pvParameters) {
    uint8_t data[128];

    while (1)
    {
        int len = uart_read_bytes(UART_PORT, data, sizeof(data) - 1, pdMS_TO_TICKS(1000));
        if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG_LVGL, "UART RX");
            if (len > 0) {
                data[len] = '\0';
                updateUserInterface((lv_obj_t *)pvParameters, (char *) data);
                ESP_LOGI(TAG_I2C, "I2C SENDED");
            }
            xSemaphoreGive(lvgl_mutex);
        } else {
            ESP_LOGI(TAG_LVGL, "Mutex FAIL");
            return;
        }
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void updateUserInterface(lv_obj_t *label, char *text) {
    // Texto a ser exibido
    lv_label_set_text(label, text);
}

void configUart(void *pvParameters) {
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
}