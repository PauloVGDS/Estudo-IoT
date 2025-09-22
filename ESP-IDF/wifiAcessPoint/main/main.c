#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"

static const char *TAG = "EVENT";

void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT || event_base == IP_EVENT) {
        switch (event_id)
        {
        case ESP_NETIF_IP_EVENT_GOT_IP:
            ESP_LOGI(TAG, "IP Obtido!");
            ip_event_got_ip_t *ip_struct = (ip_event_got_ip_t *)event_data;
            const esp_netif_ip_info_t *ip_info = &ip_struct->ip_info;
            ESP_LOGI(TAG, "Informações do IP");
            ESP_LOGI(TAG, "IP: " IPSTR, IP2STR(&ip_info->ip));
            ESP_LOGI(TAG, "Gateway: " IPSTR, IP2STR(&ip_info->gw));
            ESP_LOGI(TAG, "Mascara de Rede: " IPSTR, IP2STR(&ip_info->netmask));
            break;
        case WIFI_EVENT_AP_START:
            ESP_LOGI(TAG, "Acess Point iniciado!!!");
            break;

        case WIFI_EVENT_AP_STACONNECTED:
            ESP_LOGI(TAG, "Um dispositivo se aconectou ao AP!!!");
            break;
        case WIFI_EVENT_AP_STADISCONNECTED:
            ESP_LOGI(TAG, "Um dispositivo se desconectou do AP!!!");
            break;
        }
    }
}



void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_ap();

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &event_handler, NULL);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "ESP32_AP",
            .ssid_len = strlen("ESP32_AP"),
            .channel = 1,
            .max_connection = 4,
            .authmode = WIFI_AUTH_OPEN // sem senha
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}
