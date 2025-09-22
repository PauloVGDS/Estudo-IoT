/*
 * ESP8266 Endpoint Device - HTTP Controlled Device
 * 
 * Este código transforma um ESP8266 em um dispositivo controlável
 * via HTTP que pode ser usado como endpoint por um Matter Bridge
 * 
 * Hardware: ESP8266 (NodeMCU, Wemos D1, etc.)
 * IDE: Arduino IDE
 * Comunicação: HTTP Server
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// ========== CONFIGURAÇÕES DE REDE ==========
const char* ssid = "your-ssid";          // Mesmo SSID do bridge
const char* password = "your-password";  // Mesma senha do bridge

// ========== CONFIGURAÇÕES DO DISPOSITIVO ==========
// Configure um IP fixo para cada ESP8266 endpoint
IPAddress local_IP(192, 168, 1, 100);    // Mude para 101, 102, etc. para outros ESP8266s
IPAddress gateway(192, 168, 1, 1);       // Gateway da sua rede
IPAddress subnet(255, 255, 255, 0);      // Máscara de sub-rede
IPAddress primaryDNS(8, 8, 8, 8);        // DNS primário (opcional)

// ========== CONFIGURAÇÕES DE HARDWARE ESP8266 ==========
const uint8_t LED_PIN = LED_BUILTIN;     // LED interno (GPIO 2 no NodeMCU)
const uint8_t RELAY_PIN = D5;            // GPIO 14 - Pino do relé
const uint8_t STATUS_LED = D4;           // GPIO 2 - LED de status externo

// ========== SERVIDOR WEB ==========
ESP8266WebServer server(80);

// ========== VARIÁVEIS DE ESTADO ==========
bool deviceState = false;                // Estado atual do dispositivo
String deviceName = "ESP8266-Device-1";  // Nome do dispositivo
unsigned long lastHeartbeat = 0;         // Último heartbeat

// ========== FUNÇÕES DE CONTROLE ==========

void setDeviceState(bool newState) {
  deviceState = newState;
  
  // Controlar LED (invertido no ESP8266 built-in)
  digitalWrite(LED_PIN, newState ? LOW : HIGH);  // LED_BUILTIN é invertido
  
  // Controlar relé
  digitalWrite(RELAY_PIN, newState ? HIGH : LOW);
  
  // LED de status externo (não invertido)
  digitalWrite(STATUS_LED, newState ? HIGH : LOW);
  
  Serial.print("ESP8266 Device state changed to: ");
  Serial.println(newState ? "ON" : "OFF");
}

// ========== HANDLERS HTTP ==========

// Handler para controlar o dispositivo
void handleControl() {
  String stateParam = server.arg("state");
  
  if (stateParam == "") {
    server.send(400, "text/plain", "Missing 'state' parameter");
    return;
  }
  
  bool newState = false;
  if (stateParam == "on" || stateParam == "true" || stateParam == "1") {
    newState = true;
  } else if (stateParam == "off" || stateParam == "false" || stateParam == "0") {
    newState = false;
  } else {
    server.send(400, "text/plain", "Invalid state. Use 'on' or 'off'");
    return;
  }
  
  setDeviceState(newState);
  
  // Resposta JSON (usando StaticJsonDocument para ESP8266)
  StaticJsonDocument<200> response;
  response["status"] = "OK";
  response["device"] = deviceName;
  response["state"] = newState ? "on" : "off";
  response["timestamp"] = millis();
  
  String responseStr;
  serializeJson(response, responseStr);
  
  server.send(200, "application/json", responseStr);
  
  Serial.println("HTTP Control - State: " + String(newState ? "ON" : "OFF"));
}

// Handler para verificar status
void handleStatus() {
  StaticJsonDocument<300> status;
  
  status["device"] = deviceName;
  status["state"] = deviceState ? "on" : "off";
  status["uptime"] = millis();
  status["wifi_rssi"] = WiFi.RSSI();
  status["free_heap"] = ESP.getFreeHeap();
  status["ip_address"] = WiFi.localIP().toString();
  status["chip_id"] = ESP.getChipId();  // Específico do ESP8266
  
  String statusStr;
  serializeJson(status, statusStr);
  
  server.send(200, "application/json", statusStr);
}

// Handler para página principal (simplificada para ESP8266)
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<title>ESP8266 Endpoint Device</title>";
  html += "<meta charset='UTF-8'>";
  html += "<style>body{font-family:Arial;text-align:center;margin:50px;}";
  html += ".button{padding:15px 30px;margin:10px;font-size:18px;}";
  html += ".on{background:#4CAF50;color:white;}.off{background:#f44336;color:white;}";
  html += ".status{padding:20px;background:#f0f0f0;margin:20px 0;}</style>";
  html += "</head><body>";
  html += "<h1>ESP8266 Endpoint Device</h1>";
  html += "<div class='status'>";
  html += "<h3>Device: " + deviceName + "</h3>";
  html += "<p>State: <strong id='state'>" + String(deviceState ? "ON" : "OFF") + "</strong></p>";
  html += "<p>IP: " + WiFi.localIP().toString() + "</p>";
  html += "<p>Uptime: " + String(millis() / 1000) + " seconds</p>";
  html += "<p>Free Heap: " + String(ESP.getFreeHeap()) + " bytes</p>";
  html += "</div>";
  html += "<button class='button on' onclick='control(\"on\")'>Turn ON</button>";
  html += "<button class='button off' onclick='control(\"off\")'>Turn OFF</button>";
  html += "<script>";
  html += "function control(state){fetch('/control?state='+state).then(r=>r.json()).then(d=>{";
  html += "document.getElementById('state').textContent=d.state.toUpperCase();";
  html += "alert('Device turned '+d.state.toUpperCase());}).catch(e=>alert('Error: '+e));}";
  html += "setInterval(()=>{fetch('/status').then(r=>r.json()).then(d=>{";
  html += "document.getElementById('state').textContent=d.state.toUpperCase();});},5000);";
  html += "</script></body></html>";
  
  server.send(200, "text/html", html);
}

// Handler para info do dispositivo (adaptado para ESP8266)
void handleInfo() {
  StaticJsonDocument<400> info;
  
  info["device_name"] = deviceName;
  info["chip_id"] = ESP.getChipId();
  info["cpu_freq"] = ESP.getCpuFreqMHz();
  info["flash_size"] = ESP.getFlashChipSize();
  info["free_heap"] = ESP.getFreeHeap();
  info["sdk_version"] = ESP.getSdkVersion();
  info["core_version"] = ESP.getCoreVersion();
  info["wifi_mac"] = WiFi.macAddress();
  info["current_state"] = deviceState ? "on" : "off";
  
  String infoStr;
  serializeJson(info, infoStr);
  
  server.send(200, "application/json", infoStr);
}

// ========== FUNÇÕES DE INICIALIZAÇÃO ==========

void initializeHardware() {
  Serial.println("Initializing ESP8266 hardware...");
  
  // Configurar pinos
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  
  // Estado inicial OFF
  setDeviceState(false);
  
  Serial.printf("ESP8266 Hardware initialized:\n");
  Serial.printf("- LED Pin: %d (Built-in, inverted)\n", LED_PIN);
  Serial.printf("- Relay Pin: %d\n", RELAY_PIN);
  Serial.printf("- Status LED: %d\n", STATUS_LED);
}

void initializeWiFi() {
  Serial.print("Configuring static IP: ");
  Serial.println(local_IP);
  
  // Configurar IP estático
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
    Serial.println("Failed to configure static IP");
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    // Piscar LED durante conexão
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  
  // LED fixo após conectar
  digitalWrite(LED_PIN, HIGH);
  
  Serial.println("\nWiFi connected!");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("Signal strength: %d dBm\n", WiFi.RSSI());
  Serial.printf("Chip ID: %u\n", ESP.getChipId());
}

void initializeWebServer() {
  Serial.println("Starting web server...");
  
  // Configurar rotas
  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.on("/status", handleStatus);
  server.on("/info", handleInfo);
  
  // Handler para rotas não encontradas
  server.onNotFound([]() {
    server.send(404, "text/plain", "Endpoint not found");
  });
  
  server.begin();
  Serial.println("Web server started on port 80");
  Serial.println("Available endpoints:");
  Serial.println("- GET  /         - Web interface");
  Serial.println("- GET  /control?state=on|off - Control device");
  Serial.println("- GET  /status   - Device status");
  Serial.println("- GET  /info     - Device information");
}

// ========== FUNÇÕES PRINCIPAIS ==========

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("========================================");
  Serial.printf("    %s\n", deviceName.c_str());
  Serial.println("    HTTP Controlled ESP8266 Endpoint");
  Serial.println("========================================");
  
  initializeHardware();
  initializeWiFi();
  initializeWebServer();
  
  Serial.println("\n*** ESP8266 DEVICE READY ***");
  Serial.printf("Access web interface: http://%s\n", WiFi.localIP().toString().c_str());
  Serial.println("Device is ready to receive commands from Matter Bridge");
  Serial.println("========================================");
}

void loop() {
  // Processar requisições HTTP
  server.handleClient();
  
  // Heartbeat periódico
  if (millis() - lastHeartbeat > 30000) { // A cada 30 segundos
    Serial.printf("Heartbeat - State: %s, Free Heap: %d, RSSI: %d dBm\n", 
                  deviceState ? "ON" : "OFF", 
                  ESP.getFreeHeap(), 
                  WiFi.RSSI());
    lastHeartbeat = millis();
  }
  
  // Verificar conexão WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected! Reconnecting...");
    digitalWrite(STATUS_LED, LOW);
    WiFi.reconnect();
    delay(5000);
  }
  
  // Yield para ESP8266 (importante!)
  yield();
  delay(10);
}

/*
 * ========== DIFERENÇAS ESP8266 vs ESP32 ==========
 * 
 * 1. BIBLIOTECAS:
 *    - ESP8266WiFi.h ao invés de WiFi.h
 *    - ESP8266WebServer.h ao invés de WebServer.h
 * 
 * 2. HARDWARE:
 *    - LED_BUILTIN é invertido (LOW = ligado)
 *    - Pinos diferentes (D1, D2, etc.)
 *    - Menos memória RAM (80KB vs 320KB)
 * 
 * 3. PERFORMANCE:
 *    - CPU mais lenta (80/160MHz vs 240MHz)
 *    - Menos memória para JSON
 *    - HTML simplificado para economizar RAM
 * 
 * 4. FUNCIONALIDADES:
 *    - ESP.getChipId() ao invés de getChipModel()
 *    - yield() necessário no loop
 *    - StaticJsonDocument recomendado
 * 
 * ========== MAPEAMENTO DE PINOS ESP8266 ==========
 * 
 * NodeMCU/Wemos D1:
 * - D0 = GPIO 16 (sem PWM/interrupt)
 * - D1 = GPIO 5
 * - D2 = GPIO 4
 * - D3 = GPIO 0 (FLASH button)
 * - D4 = GPIO 2 (LED_BUILTIN)
 * - D5 = GPIO 14
 * - D6 = GPIO 12
 * - D7 = GPIO 13
 * - D8 = GPIO 15
 * 
 */
