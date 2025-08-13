/*
 * ESP32 Endpoint Device - ESP-NOW Version
 * 
 * Este código usa ESP-NOW para comunicação direta com o Matter Bridge
 * sem necessidade de router WiFi - ideal para baixo consumo
 * 
 * Hardware: ESP32 (qualquer modelo)
 * IDE: Arduino IDE
 * Comunicação: ESP-NOW (peer-to-peer)
 */

#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>

// ========== CONFIGURAÇÃO DO DISPOSITIVO ==========
const String deviceName = "ESP32-Device-1";    // Nome único para cada dispositivo
const uint8_t DEVICE_ID = 1;                   // ID único (1, 2, 3, etc.)

// MAC Address do Matter Bridge (você precisa descobrir e configurar)
uint8_t bridgeMAC[] = {0x24, 0x6F, 0x28, 0x12, 0x34, 0x56}; // SUBSTITUA PELO MAC REAL

// ========== CONFIGURAÇÕES DE HARDWARE ==========
const uint8_t LED_PIN = 2;               // Pino do LED controlado
const uint8_t RELAY_PIN = 5;             // Pino do relé (opcional)
const uint8_t STATUS_LED = 15;           // LED de status

// ========== ESTRUTURAS DE DADOS ESP-NOW ==========
typedef struct {
  uint8_t deviceId;
  uint8_t command;      // 0=OFF, 1=ON, 2=STATUS_REQUEST
  uint32_t timestamp;
  char deviceName[32];
} BridgeCommand;

typedef struct {
  uint8_t deviceId;
  uint8_t state;        // 0=OFF, 1=ON
  uint8_t online;       // 0=OFFLINE, 1=ONLINE
  int16_t rssi;
  uint32_t uptime;
  char deviceName[32];
} DeviceResponse;

// ========== VARIÁVEIS GLOBAIS ==========
bool deviceState = false;
unsigned long lastHeartbeat = 0;
bool bridgeConnected = false;

// ========== FUNÇÕES DE CONTROLE ==========

void setDeviceState(bool newState) {
  deviceState = newState;
  
  // Controlar hardware
  digitalWrite(LED_PIN, newState ? HIGH : LOW);
  digitalWrite(RELAY_PIN, newState ? HIGH : LOW);
  
  // Feedback visual
  if (newState) {
    digitalWrite(STATUS_LED, HIGH);
  } else {
    // Piscar para indicar OFF
    for (int i = 0; i < 3; i++) {
      digitalWrite(STATUS_LED, HIGH);
      delay(100);
      digitalWrite(STATUS_LED, LOW);
      delay(100);
    }
  }
  
  Serial.print("Device state changed to: ");
  Serial.println(newState ? "ON" : "OFF");
  
  // Enviar confirmação para o bridge
  sendStatusToBridge();
}

// ========== ESP-NOW CALLBACKS ==========

// Callback quando dados são recebidos
void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len) {
  BridgeCommand command;
  memcpy(&command, incomingData, sizeof(command));
  
  Serial.printf("Command received from bridge: ID=%d, CMD=%d\n", 
                command.deviceId, command.command);
  
  // Verificar se o comando é para este dispositivo
  if (command.deviceId != DEVICE_ID) {
    Serial.println("Command not for this device - ignoring");
    return;
  }
  
  bridgeConnected = true; // Bridge está ativo
  
  switch (command.command) {
    case 0: // OFF
      setDeviceState(false);
      break;
      
    case 1: // ON
      setDeviceState(true);
      break;
      
    case 2: // STATUS REQUEST
      sendStatusToBridge();
      break;
      
    default:
      Serial.println("Unknown command received");
      break;
  }
}

// Callback quando dados são enviados
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Data send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

// ========== FUNÇÕES ESP-NOW ==========

void sendStatusToBridge() {
  DeviceResponse response;
  
  response.deviceId = DEVICE_ID;
  response.state = deviceState ? 1 : 0;
  response.online = 1;
  response.rssi = WiFi.RSSI();
  response.uptime = millis() / 1000;
  strncpy(response.deviceName, deviceName.c_str(), sizeof(response.deviceName));
  
  esp_err_t result = esp_now_send(bridgeMAC, (uint8_t*)&response, sizeof(response));
  
  if (result == ESP_OK) {
    Serial.println("Status sent to bridge successfully");
  } else {
    Serial.println("Error sending status to bridge");
  }
}

void initializeESPNOW() {
  Serial.println("Initializing ESP-NOW...");
  
  // Inicializar WiFi em modo STA
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
  
  // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Registrar callbacks
  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(onDataReceived);
  
  // Adicionar peer (bridge)
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, bridgeMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add bridge as peer");
    return;
  }
  
  Serial.println("ESP-NOW initialized successfully");
  Serial.print("Connected to bridge MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", bridgeMAC[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}

// ========== FUNÇÕES DE INICIALIZAÇÃO ==========

void initializeHardware() {
  Serial.println("Initializing hardware...");
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  
  // Estado inicial OFF
  setDeviceState(false);
  
  // Sequência de inicialização nos LEDs
  for (int i = 0; i < 5; i++) {
    digitalWrite(STATUS_LED, HIGH);
    delay(200);
    digitalWrite(STATUS_LED, LOW);
    delay(200);
  }
  
  Serial.printf("Hardware initialized - Device ID: %d\n", DEVICE_ID);
}

// ========== FUNÇÕES PRINCIPAIS ==========

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("========================================");
  Serial.printf("    %s (ID: %d)\n", deviceName.c_str(), DEVICE_ID);
  Serial.println("    ESP-NOW Controlled Endpoint");
  Serial.println("========================================");
  
  initializeHardware();
  initializeESPNOW();
  
  Serial.println("\n*** DEVICE READY ***");
  Serial.println("Waiting for commands from Matter Bridge via ESP-NOW");
  Serial.println("========================================");
  
  // Enviar status inicial para o bridge
  delay(2000);
  sendStatusToBridge();
}

void loop() {
  // Heartbeat periódico
  if (millis() - lastHeartbeat > 30000) { // A cada 30 segundos
    Serial.printf("Heartbeat - State: %s, Free Heap: %d\n", 
                  deviceState ? "ON" : "OFF", 
                  ESP.getFreeHeap());
    
    // Enviar status para manter conexão com bridge
    sendStatusToBridge();
    lastHeartbeat = millis();
  }
  
  // Verificar se bridge ainda está conectado
  static unsigned long lastBridgeContact = 0;
  if (bridgeConnected) {
    lastBridgeContact = millis();
    bridgeConnected = false; // Reset flag
  }
  
  // Se não receber dados do bridge por 2 minutos, indicar desconectado
  if (millis() - lastBridgeContact > 120000) {
    // Piscar LED de status para indicar desconexão
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 1000) {
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
      lastBlink = millis();
    }
  }
  
  delay(100);
}

/*
 * ========== INSTRUÇÕES DE CONFIGURAÇÃO ==========
 * 
 * 1. DESCOBRIR MAC ADDRESS DO BRIDGE:
 *    - No bridge, adicione: Serial.println(WiFi.macAddress());
 *    - Copie o MAC e substitua em bridgeMAC[]
 * 
 * 2. CONFIGURAR DISPOSITIVOS MÚLTIPLOS:
 *    - Mude DEVICE_ID para cada ESP32 (1, 2, 3, etc.)
 *    - Mude deviceName para identificação
 * 
 * 3. VANTAGENS ESP-NOW:
 *    - Comunicação direta, sem router
 *    - Baixo consumo de energia
 *    - Alcance até 200m em campo aberto
 *    - Latência muito baixa (~5ms)
 * 
 * 4. LIMITAÇÕES:
 *    - Máximo 20 peers por dispositivo
 *    - Mesma frequência WiFi (2.4GHz)
 *    - Sem segurança criptográfica nativa
 * 
 */
