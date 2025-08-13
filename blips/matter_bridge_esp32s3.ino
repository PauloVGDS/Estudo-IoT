/*
 * Matter Bridge ESP32-S3 - Dual On/Off Light Bridge
 * 
 * Este código implementa um Matter Bridge com 2 endpoints on/off
 * que controlam outros ESP32s remotos via HTTP/WiFi
 * 
 * Hardware: ESP32-S3
 * IDE: Arduino IDE
 * Framework: ESP32 Arduino Core with Matter support
 */

#include <Matter.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ========== CONFIGURAÇÕES DE REDE ==========
const char* ssid = "your-ssid";          // Substitua pelo seu SSID WiFi
const char* password = "your-password";  // Substitua pela sua senha WiFi

// ========== CONFIGURAÇÕES DOS ESP32 REMOTOS ==========
// IPs dos ESP32s que serão controlados como endpoints
const char* DEVICE_1_IP = "192.168.1.100";  // IP do primeiro ESP32
const char* DEVICE_2_IP = "192.168.1.101";  // IP do segundo ESP32
const int DEVICE_PORT = 80;                 // Porta do servidor HTTP nos ESP32s

// URLs para controlar os dispositivos remotos
String device1_url = "http://" + String(DEVICE_1_IP) + ":" + String(DEVICE_PORT);
String device2_url = "http://" + String(DEVICE_2_IP) + ":" + String(DEVICE_PORT);

// ========== CONFIGURAÇÕES DE HARDWARE ==========
// Pinos dos LEDs que representam os dispositivos bridgeados
const uint8_t LED_DEVICE_1 = 2;   // LED interno ou GPIO 2
const uint8_t LED_DEVICE_2 = 15;  // GPIO 15 para segundo dispositivo

// Pino do botão para reset/decommissioning
const uint8_t RESET_BUTTON = 0;   // Botão BOOT do ESP32-S3

// ========== MATTER ENDPOINTS ==========
// Criando 2 endpoints Matter para simular dispositivos bridgeados
MatterOnOffLight bridgedDevice1;   // Primeiro dispositivo (ex: lâmpada Zigbee)
MatterOnOffLight bridgedDevice2;   // Segundo dispositivo (ex: tomada Z-Wave)

// ========== VARIÁVEIS DE CONTROLE ==========
// Estados dos dispositivos bridgeados
bool device1State = false;
bool device2State = false;

// Controle do botão de reset
unsigned long buttonPressTime = 0;
bool buttonPressed = false;
const unsigned long RESET_TIMEOUT = 5000; // 5 segundos para reset

// ========== CALLBACKS DOS DISPOSITIVOS BRIDGEADOS ==========

// Função para enviar comando HTTP para ESP32 remoto
bool sendHTTPCommand(const String& deviceURL, bool state, const String& deviceName) {
  HTTPClient http;
  bool success = false;
  
  // Montar URL com comando
  String url = deviceURL + "/control?state=" + (state ? "on" : "off");
  
  Serial.print("Sending command to " + deviceName + ": ");
  Serial.println(url);
  
  http.begin(url);
  http.setTimeout(3000); // Timeout de 3 segundos
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    Serial.println("Response: " + response);
    
    // Verificar se o comando foi aceito
    if (response.indexOf("OK") >= 0 || response.indexOf("success") >= 0) {
      success = true;
      Serial.println("✓ Command sent successfully");
    } else {
      Serial.println("✗ Device rejected command");
    }
  } else {
    Serial.printf("✗ HTTP Error: %d\n", httpCode);
    Serial.println("Device may be offline or unreachable");
  }
  
  http.end();
  return success;
}

// Callback para controlar o Dispositivo 1 (ESP32 Remoto)
bool onDevice1StateChange(bool newState) {
  device1State = newState;
  
  Serial.print("Bridge: Controlling Remote ESP32 Device 1 -> ");
  Serial.println(newState ? "ON" : "OFF");
  
  // Enviar comando HTTP para o ESP32 remoto
  bool success = sendHTTPCommand(device1_url, newState, "Device 1");
  
  if (success) {
    // Atualizar LED local como indicador
    digitalWrite(LED_DEVICE_1, newState ? HIGH : LOW);
  } else {
    // Se falhou, reverter estado
    device1State = !newState;
    Serial.println("Command failed - reverting state");
  }
  
  return success;
}

// Callback para controlar o Dispositivo 2 (ESP32 Remoto)
bool onDevice2StateChange(bool newState) {
  device2State = newState;
  
  Serial.print("Bridge: Controlling Remote ESP32 Device 2 -> ");
  Serial.println(newState ? "ON" : "OFF");
  
  // Enviar comando HTTP para o ESP32 remoto
  bool success = sendHTTPCommand(device2_url, newState, "Device 2");
  
  if (success) {
    // Atualizar LED local como indicador
    digitalWrite(LED_DEVICE_2, newState ? HIGH : LOW);
  } else {
    // Se falhou, reverter estado
    device2State = !newState;
    Serial.println("Command failed - reverting state");
  }
  
  return success;
}

// ========== GERENCIADOR DE EVENTOS MATTER ==========
void onMatterEvent(matterEvent_t eventType, const chip::DeviceLayer::ChipDeviceEvent* eventInfo) {
  Serial.print("Matter Bridge Event: ");
  
  switch (eventType) {
    case MATTER_WIFI_CONNECTIVITY_CHANGE:
      Serial.println("WiFi Connectivity Change");
      break;
      
    case MATTER_INTERNET_CONNECTIVITY_CHANGE:
      Serial.print("Internet Connectivity Change - ");
      if (eventInfo->InternetConnectivityChange.IPv4 == chip::DeviceLayer::ConnectivityChange::kConnectivity_Established) {
        Serial.println("IPv4 Connected");
      } else if (eventInfo->InternetConnectivityChange.IPv6 == chip::DeviceLayer::ConnectivityChange::kConnectivity_Established) {
        Serial.println("IPv6 Connected");
      } else {
        Serial.println("Connection Lost");
      }
      break;
      
    case MATTER_COMMISSIONING_COMPLETE:
      Serial.println("Bridge Commissioned Successfully!");
      Serial.println("Bridge is now available in Matter network");
      break;
      
    case MATTER_COMMISSIONING_SESSION_STARTED:
      Serial.println("Commissioning Session Started");
      break;
      
    case MATTER_COMMISSIONING_SESSION_STOPPED:
      Serial.println("Commissioning Session Stopped");
      break;
      
    case MATTER_COMMISSIONING_WINDOW_OPEN:
      Serial.println("Commissioning Window Opened");
      break;
      
    case MATTER_COMMISSIONING_WINDOW_CLOSED:
      Serial.println("Commissioning Window Closed");
      break;
      
    case MATTER_SERVER_READY:
      Serial.println("Matter Server Ready - Bridge operational");
      break;
      
    case MATTER_FABRIC_COMMITTED:
      Serial.println("Fabric Committed - Bridge joined network");
      break;
      
    default:
      Serial.print("Event ID: 0x");
      Serial.println(eventType, HEX);
      break;
  }
}

// ========== FUNÇÕES DE INICIALIZAÇÃO ==========

void initializeHardware() {
  Serial.println("Initializing Bridge Hardware...");
  
  // Configurar pinos dos LEDs
  pinMode(LED_DEVICE_1, OUTPUT);
  pinMode(LED_DEVICE_2, OUTPUT);
  digitalWrite(LED_DEVICE_1, LOW);
  digitalWrite(LED_DEVICE_2, LOW);
  
  // Configurar botão de reset
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  
  Serial.println("Hardware initialized:");
  Serial.printf("- Device 1 LED: GPIO %d\n", LED_DEVICE_1);
  Serial.printf("- Device 2 LED: GPIO %d\n", LED_DEVICE_2);
  Serial.printf("- Reset Button: GPIO %d\n", RESET_BUTTON);
}

void initializeWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.enableIPv6(true);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("IPv4 Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("IPv6 Address: ");
  Serial.println(WiFi.linkLocalIPv6());
}

void initializeMatterBridge() {
  Serial.println("Initializing Matter Bridge...");
  
  // Inicializar endpoints Matter
  bridgedDevice1.begin();
  bridgedDevice2.begin();
  
  // Associar callbacks aos endpoints
  bridgedDevice1.onChange(onDevice1StateChange);
  bridgedDevice2.onChange(onDevice2StateChange);
  
  // Configurar callback de eventos Matter
  Matter.onEvent(onMatterEvent);
  
  // Inicializar Matter (sempre por último)
  Matter.begin();
  
  Serial.println("Matter Bridge initialized with 2 endpoints:");
  Serial.println("- Endpoint 1: Simulated Zigbee Bulb");
  Serial.println("- Endpoint 2: Simulated Z-Wave Outlet");
}

// ========== FUNÇÕES PRINCIPAIS ==========

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("========================================");
  Serial.println("    Matter Bridge ESP32-S3 v1.0");
  Serial.println("    Dual On/Off Light Bridge");
  Serial.println("========================================");
  
  // Inicializar componentes
  initializeHardware();
  initializeWiFi();
  initializeMatterBridge();
  
  // Verificar estado de comissionamento
  if (!Matter.isDeviceCommissioned()) {
    Serial.println("\n*** BRIDGE NOT COMMISSIONED ***");
    Serial.println("Use your Matter app to add this bridge:");
    Serial.printf("Manual Code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR Code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
    Serial.println("========================================");
  } else {
    Serial.println("\n*** BRIDGE ALREADY COMMISSIONED ***");
    Serial.println("Bridge is ready to control devices!");
    Serial.println("========================================");
  }
}

void loop() {
  // Verificar botão de reset/decommissioning
  handleResetButton();
  
  // Atualizar estados dos dispositivos (simular sincronização)
  updateDeviceStates();
  
  // Status periódico
  static unsigned long lastStatusTime = 0;
  if (millis() - lastStatusTime > 30000) { // A cada 30 segundos
    printBridgeStatus();
    lastStatusTime = millis();
  }
  
  delay(100); // Pequeno delay para não sobrecarregar
}

// ========== FUNÇÕES AUXILIARES ==========

void handleResetButton() {
  // Detectar pressão do botão
  if (digitalRead(RESET_BUTTON) == LOW && !buttonPressed) {
    buttonPressed = true;
    buttonPressTime = millis();
    Serial.println("Reset button pressed...");
  }
  
  // Detectar liberação do botão
  if (digitalRead(RESET_BUTTON) == HIGH && buttonPressed) {
    buttonPressed = false;
    Serial.println("Reset button released.");
  }
  
  // Verificar tempo de pressão para reset
  if (buttonPressed && (millis() - buttonPressTime > RESET_TIMEOUT)) {
    Serial.println("\n*** DECOMMISSIONING BRIDGE ***");
    Serial.println("Bridge will be removed from Matter network...");
    Matter.decommission();
    buttonPressTime = millis(); // Evitar múltiplos resets
    
    // Piscar LEDs para indicar reset
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_DEVICE_1, HIGH);
      digitalWrite(LED_DEVICE_2, HIGH);
      delay(200);
      digitalWrite(LED_DEVICE_1, LOW);
      digitalWrite(LED_DEVICE_2, LOW);
      delay(200);
    }
  }
}

void updateDeviceStates() {
  // Verificar status dos ESP32s remotos periodicamente
  static unsigned long lastCheck = 0;
  
  if (millis() - lastCheck > 10000) { // Verificar a cada 10 segundos
    checkRemoteDeviceStatus();
    lastCheck = millis();
  }
}

// Verificar status dos dispositivos remotos
void checkRemoteDeviceStatus() {
  // Verificar Device 1
  HTTPClient http1;
  String statusUrl1 = device1_url + "/status";
  http1.begin(statusUrl1);
  http1.setTimeout(2000);
  
  int code1 = http1.GET();
  if (code1 == HTTP_CODE_OK) {
    String response1 = http1.getString();
    bool remoteState1 = (response1.indexOf("on") >= 0 || response1.indexOf("true") >= 0);
    
    // Sincronizar estado se diferente
    if (remoteState1 != device1State) {
      device1State = remoteState1;
      digitalWrite(LED_DEVICE_1, remoteState1 ? HIGH : LOW);
      // Atualizar Matter endpoint
      bridgedDevice1.updateState(remoteState1);
      Serial.println("Device 1 state synchronized: " + String(remoteState1 ? "ON" : "OFF"));
    }
  } else {
    Serial.println("Device 1 status check failed - may be offline");
  }
  http1.end();
  
  // Verificar Device 2
  HTTPClient http2;
  String statusUrl2 = device2_url + "/status";
  http2.begin(statusUrl2);
  http2.setTimeout(2000);
  
  int code2 = http2.GET();
  if (code2 == HTTP_CODE_OK) {
    String response2 = http2.getString();
    bool remoteState2 = (response2.indexOf("on") >= 0 || response2.indexOf("true") >= 0);
    
    // Sincronizar estado se diferente
    if (remoteState2 != device2State) {
      device2State = remoteState2;
      digitalWrite(LED_DEVICE_2, remoteState2 ? HIGH : LOW);
      // Atualizar Matter endpoint
      bridgedDevice2.updateState(remoteState2);
      Serial.println("Device 2 state synchronized: " + String(remoteState2 ? "ON" : "OFF"));
    }
  } else {
    Serial.println("Device 2 status check failed - may be offline");
  }
  http2.end();
}

void printBridgeStatus() {
  Serial.println("\n--- Matter Bridge Status ---");
  Serial.printf("Commissioned: %s\n", Matter.isDeviceCommissioned() ? "YES" : "NO");
  Serial.printf("WiFi: %s (RSSI: %d dBm)\n", 
                WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected",
                WiFi.RSSI());
  Serial.printf("Device 1 State: %s\n", device1State ? "ON" : "OFF");
  Serial.printf("Device 2 State: %s\n", device2State ? "ON" : "OFF");
  Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
  Serial.println("----------------------------");
}

/*
 * ========== NOTAS DE IMPLEMENTAÇÃO ==========
 * 
 * Para implementar um bridge real, você precisa:
 * 
 * 1. PROTOCOLOS DE COMUNICAÇÃO:
 *    - Adicionar bibliotecas para Zigbee, Z-Wave, etc.
 *    - Implementar drivers de comunicação específicos
 * 
 * 2. DESCOBERTA DE DISPOSITIVOS:
 *    - Implementar scan automático de dispositivos
 *    - Criar endpoints Matter dinamicamente
 * 
 * 3. SINCRONIZAÇÃO BIDIRECIONAL:
 *    - Monitorar mudanças nos dispositivos externos
 *    - Atualizar estados no Matter quando dispositivos mudam externamente
 * 
 * 4. PERSISTÊNCIA:
 *    - Salvar configurações dos dispositivos na flash
 *    - Recuperar dispositivos após reset
 * 
 * 5. INTERFACE DE CONFIGURAÇÃO:
 *    - Web server para adicionar/remover dispositivos
 *    - API REST para gerenciamento
 * 
 * Exemplo de expansão para Zigbee:
 * 
 * #include <ZigbeeESP32.h>
 * 
 * void discoverZigbeeDevices() {
 *   auto devices = zigbee.scanDevices();
 *   for (auto& device : devices) {
 *     if (device.type == ZIGBEE_ON_OFF_LIGHT) {
 *       createMatterEndpoint(device);
 *     }
 *   }
 * }
 * 
 */
