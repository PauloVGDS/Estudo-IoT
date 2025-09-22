#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
// Lib Wi-Fi
#include "esp_wifi.h"
// Lib para tratar de eventos
#include "esp_event.h"
// Lib para guardar as informações do wifi na memória NVS(Non Volatile Storage)
#include "nvs_flash.h"
#include "esp_netif.h"

#define WIFI_SSID "Pedro"
#define WIFI_PASS "91204673"

static const char *TAG = "Wi-Fi";


void scan_wifi_networks() {
    // Configurar a varredura
    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = true
    };

    while (1)
    {
        // Iniciar a varredura
        ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));

        // Obter o número de redes encontradas
        uint16_t ap_count = 0;
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
        ESP_LOGI(TAG, "Número de redes encontradas: %d", ap_count);

        // Obter os detalhes das redes
        wifi_ap_record_t ap_info[10]; // Limite de 10 redes
        if (ap_count > 10) ap_count = 10; // Garantir que não exceda o tamanho do array
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_count, ap_info));

        // Exibir informações das redes
        for (int i = 0; i < ap_count; i++) {
            ESP_LOGI(TAG, "SSID: %s, RSSI: %d, Canal: %d, Authmode: %d",
                    ap_info[i].ssid, ap_info[i].rssi, ap_info[i].primary, ap_info[i].authmode);
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
    
}

void app_main(void) {
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();


    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS
        }
    };

    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    xTaskCreate(scan_wifi_networks, "SCAN", 4096, NULL, 5, NULL);

}