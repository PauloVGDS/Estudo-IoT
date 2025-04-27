// Modo Station

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

static const char *TAG = "EVENT";

void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

void app_main(void) {
    // Inicializando interfaces
    nvs_flash_init();
    esp_netif_init();
    // Iniciando Loop de Eventos
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL);

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
    
}


void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT || event_base == IP_EVENT) {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
            ESP_LOGI(TAG, "Wi-Fi iniciado, conectando...");
            esp_wifi_connect();
            break;
        
        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "Wi-Fi desconectado, tentando reconectar...");
            esp_wifi_connect();
            break;

        case IP_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "Endereço IP obtido!");
            ip_event_got_ip_t *ip_struct = (ip_event_got_ip_t *)event_data;
            const esp_netif_ip_info_t *ip_info = &ip_struct->ip_info;
            ESP_LOGI(TAG, "IP:" IPSTR, IP2STR(&ip_info->ip));
            break;

        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "Wi-Fi conectado com sucesso!");
            wifi_event_sta_connected_t *wifi_struct = (wifi_event_sta_connected_t *)event_data;
            ESP_LOGI(TAG, "INFORMAÇÕES DA REDE");
            ESP_LOGI(TAG, "Nome da Rede: %s", (char *)wifi_struct->ssid);
            ESP_LOGI(TAG, "Tamanho da Rede: %i", wifi_struct->ssid_len);
            ESP_LOGI(TAG, "ID de Autenticação: %i", wifi_struct->aid);
            ESP_LOGI(TAG, "BSSID da Rede: %s", wifi_struct->bssid);
            break;
        default:
            ESP_LOGV(TAG, "%s", event_base);
            break;
        }
    }
}