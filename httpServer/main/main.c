#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Lib para HTTP
#include "esp_http_client.h"
// Lib Wi-Fi
#include "esp_wifi.h"
// Lib para tratar de eventos
#include "esp_event.h"
#include "esp_mac.h"
// Lib para guardar as informações do wifi na memória NVS(Non Volatile Storage)
#include "nvs_flash.h"
#include "esp_netif.h"

#define WIFI_SSID "Pedro"
#define WIFI_PASS "91204673"
#define MAX_RETRIES 6
//#define IPSTRING "%d.%d.%d.%d"
//#define MACSTRING "%02x:%02x:%02x:%02x:%02x:%02x"

static const char *TAG_WIFI = "WIFI";
static const char *TAG_HTTP = "HTTP";
void stationMode(void *pvParameters);
void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void http_get_task(void *pvParameters);

void app_main(void) {

    // Inicializando interfaces
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    
    // Iniciando Loop de Eventos
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    // Registrando os handlers de evento
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    xTaskCreate(stationMode, "stationMode", 4096, NULL, 5, NULL);
    xTaskCreate(http_get_task, "http_get_task", 4096, NULL, 5, NULL);
    
}

void stationMode(void *pvParameters) {
        // Instanciando struct de configuração como default e iniciando WIFI no modo STA
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        
        // Estrutura de configuração do WIFI AP, STA e NAN.
        wifi_config_t wifi_config = {
            .sta = {
                .ssid = WIFI_SSID,
                .password = WIFI_PASS
            }
        };
    
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_start());

        while (1) {
            // Aguarda 10 segundos antes de reiniciar o loop
            vTaskDelay(600000 / portTICK_PERIOD_MS);
            ESP_LOGI(TAG_WIFI, "Loop de eventos");
        }
        

}

void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    // Condição para tratamento de eventos de WIFI e IP
    if (event_base == WIFI_EVENT || event_base == IP_EVENT) {
        switch (event_id)
        {
            // Evento de inicialização do WIFI
        case WIFI_EVENT_STA_START:
            ESP_LOGI(TAG_WIFI, "Wi-Fi iniciado, conectando...");
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;
        
            // Evento de WIFI Desconectado
        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG_WIFI, "Wi-Fi desconectado, tentando reconectar...");
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;

            // Evento de obtenção de IP
        case IP_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG_WIFI, "Endereço IP obtido!");
            ip_event_got_ip_t *ip_struct = (ip_event_got_ip_t *)event_data;
            const esp_netif_ip_info_t *ip_info = &ip_struct->ip_info;
            ESP_LOGI(TAG_WIFI, "IP:" IPSTR, IP2STR(&ip_info->ip));
            break;

            // Evento após a conexão de alguma rede WIFI
        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG_WIFI, "Wi-Fi conectado com sucesso!");
            wifi_event_sta_connected_t *wifi_struct = (wifi_event_sta_connected_t *)event_data;
            ESP_LOGI(TAG_WIFI, "INFORMAÇÕES DA REDE");
            ESP_LOGI(TAG_WIFI, "Nome da Rede: %s", (char *)wifi_struct->ssid);
            ESP_LOGI(TAG_WIFI, "Tamanho da Rede: %i", wifi_struct->ssid_len);
            ESP_LOGI(TAG_WIFI, "ID de Autenticação: %i", wifi_struct->aid);
            ESP_LOGI(TAG_WIFI, "BSSID da Rede: " MACSTR, (wifi_struct->bssid)[0], (wifi_struct->bssid)[1], (wifi_struct->bssid)[2], (wifi_struct->bssid)[3], (wifi_struct->bssid)[4], (wifi_struct->bssid)[5]);
            break;
        default:
            ESP_LOGV(TAG_WIFI, "%s", event_base);
            break;
        }


    }
} 

void http_get_task(void *pvParameters) {
    int retries = 0;
    esp_err_t err;
    esp_http_client_config_t config = {
        .url = "http://google.com",
    };

    
    do {
        esp_http_client_handle_t client = esp_http_client_init(&config);
        err = esp_http_client_perform(client);

        if (err == ESP_OK) {
            ESP_LOGI(TAG_HTTP, "Status = %d", esp_http_client_get_status_code(client));
            char buffer[1024];
            int content_length = esp_http_client_read_response(client, buffer, sizeof(buffer) - 1);
            if (content_length >= 0) {
                buffer[content_length] = 0; // Fecha a string
                ESP_LOGI(TAG_HTTP, "Conteúdo:\n%s", buffer);
            } else {
                ESP_LOGE(TAG_HTTP, "Erro ao ler a resposta");
            }
        } else {
            ESP_LOGE(TAG_HTTP, "Erro na solicitação: %s", esp_err_to_name(err));
            retries++;
            vTaskDelay(pdMS_TO_TICKS(2000)); // espera antes de tentar de novo
        }
        esp_http_client_cleanup(client);

    } while (err != ESP_OK && retries < MAX_RETRIES);
    if (retries == MAX_RETRIES) {
        ESP_LOGE(TAG_HTTP, "Falha após %d tentativas.", MAX_RETRIES);
    }

    vTaskDelete(NULL);
    
}

