// Minimal Arduino sketch for mdash.net
//
// - Install mDash library:
//   - Select "Sketch" &rarr; "Include Library" &rarr; "Manage Libraries"
//   - In the search field, type "mDash" and press Enter
//   - Click on "Install" to install the library
// - Select "Tools" → "Board" → "ESP32 Dev Module"
// - Select "Tools" → "Partitioning Scheme" → "Minimal SPIFFS"
// - Select "Tools" → "Port" → your serial port
// - Click on "Upload" button to build and flash the firmware
//
// See https://mdash.net/docs/ for the full IoT product reference impementation

#include <Arduino.h> // ALWAYS AT FIRST
#include "esp_log.h"
#include <mDash.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#define DEVICE_PASSWORD "i1ykRbf3KeCm0r6SRk99j4Q"

static const char *TAG = "INFO";
volatile bool resetRequested = false;
unsigned long pressStartTime = 0;
const unsigned long PressDuration = 7000;


void IRAM_ATTR resetISR() {
    if (digitalRead(4) == LOW) {
        resetRequested = true;  // Só sinaliza
        pressStartTime = millis();  
    }

}

void setup() {

    WiFi.mode(WIFI_AP); // explicitly set mode, esp defaults to STA+AP
    WiFiManager wm;
    Serial.begin(115200);
    
    esp_log_level_set("*", ESP_LOG_INFO);
    //ESP_LOGI(tag, "String: %s", variavel);
    ESP_LOGI(TAG, "Iniciando o setup...");
    
    pinMode(2, OUTPUT);
    pinMode(4, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(4), resetISR, FALLING);  // Liga ao pino 

    bool res;
    res = wm.autoConnect("ESP");
    

    if(!res) {
        ESP_LOGI(TAG, "Falha ao conectar ao WiFi, reiniciando...");
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        ESP_LOGI(TAG, "Conectado ao WiFi!");
        mDashBegin(DEVICE_PASSWORD);
    }
    

}

void loop() {
    delay(500);
    ESP_LOGI(TAG, "Hello World!");
    if (resetRequested && digitalRead(4) == LOW) {
        if (millis() - pressStartTime >= PressDuration)
        {
            ESP_LOGI(TAG, "Reset das credenciais Wi-Fi solicitado!");
            
            // Cria uma nova instância de WiFiManager para reset
            WiFiManager wm;
            wm.resetSettings();  // Limpa credenciais salvas  
            
            ESP_LOGI(TAG, "Credenciais resetadas. Reiniciando...");
            ESP.restart();  // Reinicia para aplicar (cria AP novamente)
            
            resetRequested = false;  // Reseta a flag (embora reinicie)    
        }
        
    } else if (digitalRead(4) == HIGH) {
        resetRequested = false;  // Botão solto, cancela o reset
        pressStartTime = 0;
    }
}
