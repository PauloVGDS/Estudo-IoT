/*
 * ESP32 Endpoint Device - HTTP Controlled Device
 * 
 * Este código transforma um ESP32 em um dispositivo controlável
 * via HTTP que pode ser usado como endpoint por um Matter Bridge
 * 
 * Hardware: ESP32 (qualquer modelo)
 * IDE: Arduino IDE
 * Comunicação: HTTP Server
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// ========== CONFIGURAÇÕES DE REDE ==========
const char* ssid = "your-ssid";          // Mesmo SSID do bridge
const char* password = "your-password";  // Mesma senha do bridge

// ========== CONFIGURAÇÕES DO DISPOSITIVO ==========
// Configure um IP fixo para cada ESP32 endpoint
IPAddress local_IP(192, 168, 1, 100);    // Mude para 101, 102, etc. para outros ESP32s
IPAddress gateway(192, 168, 1, 1);       // Gateway da sua rede
IPAddress subnet(255, 255, 255, 0);      // Máscara de sub-rede
IPAddress primaryDNS(8, 8, 8, 8);        // DNS primário (opcional)

// ========== CONFIGURAÇÕES DE HARDWARE ==========
const uint8_t LED_PIN = 2;               // Pino do LED controlado
const uint8_t RELAY_PIN = 5;             // Pino do relé (opcional)
const uint8_t STATUS_LED = 15;           // LED de status (opcional)

// ========== SERVIDOR WEB ==========
WebServer server(80);

// ========== VARIÁVEIS DE ESTADO ==========
bool deviceState = false;                // Estado atual do dispositivo
String deviceName = "ESP32-Device-1";    // Nome do dispositivo
unsigned long lastHeartbeat = 0;         // Último heartbeat

// ========== FUNÇÕES DE CONTROLE ==========

void setDeviceState(bool newState) {
  deviceState = newState;
  
  // Controlar LED
  digitalWrite(LED_PIN, newState ? HIGH : LOW);
  
  // Controlar relé (se conectado)
  digitalWrite(RELAY_PIN, newState ? HIGH : LOW);
  
  // Feedback visual no LED de status
  if (newState) {
    digitalWrite(STATUS_LED, HIGH);
  } else {
    digitalWrite(STATUS_LED, LOW);
  }
  
  Serial.print("Device state changed to: ");
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
  
  // Resposta JSON
  DynamicJsonDocument response(200);
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
  DynamicJsonDocument status(300);
  
  status["device"] = deviceName;
  status["state"] = deviceState ? "on" : "off";
  status["uptime"] = millis();
  status["wifi_rssi"] = WiFi.RSSI();
  status["free_heap"] = ESP.getFreeHeap();
  status["ip_address"] = WiFi.localIP().toString();
  
  String statusStr;
  serializeJson(status, statusStr);
  
  server.send(200, "application/json", statusStr);
}

// Handler para página principal
void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Endpoint Device</title>
    <meta charset="UTF-8">
    <style>
        body { font-family: Arial; text-align: center; margin: 50px; }
        .button { padding: 15px 30px; margin: 10px; font-size: 18px; }
        .on { background: #4CAF50; color: white; }
        .off { background: #f44336; color: white; }
        .status { padding: 20px; background: #f0f0f0; margin: 20px 0; }
    </style>
</head>
<body>
    <h1>ESP32 Endpoint Device</h1>
    <div class="status">
        <h3>Device: )" + deviceName + R"(</h3>
        <p>State: <strong id="state">)" + (deviceState ? "ON" : "OFF") + R"(</strong></p>
        <p>IP: )" + WiFi.localIP().toString() + R"(</p>
        <p>Uptime: )" + String(millis() / 1000) + R"( seconds</p>
    </div>
    
    <button class="button on" onclick="control('on')">Turn ON</button>
    <button class="button off" onclick="control('off')">Turn OFF</button>
    
    <script>
        function control(state) {
            fetch('/control?state=' + state)
                .then(response => response.json())
                .then(data => {
                    document.getElementById('state').textContent = data.state.toUpperCase();
                    alert('Device turned ' + data.state.toUpperCase());
                })
                .catch(error => alert('Error: ' + error));
        }
        
        // Atualizar status a cada 5 segundos
        setInterval(() => {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('state').textContent = data.state.toUpperCase();
                });
        }, 5000);
    </script>
</body>
</html>
)";
  
  server.send(200, "text/html", html);
}

// Handler para info do dispositivo
void handleInfo() {
  DynamicJsonDocument info(500);
  
  info["device_name"] = deviceName;
  info["chip_model"] = ESP.getChipModel();
  info["chip_revision"] = ESP.getChipRevision();
  info["cpu_freq"] = ESP.getCpuFreqMHz();
  info["flash_size"] = ESP.getFlashChipSize();
  info["free_heap"] = ESP.getFreeHeap();
  info["sdk_version"] = ESP.getSdkVersion();
  info["wifi_mac"] = WiFi.macAddress();
  info["current_state"] = deviceState ? "on" : "off";
  
  String infoStr;
  serializeJson(info, infoStr);
  
  server.send(200, "application/json", infoStr);
}

// ========== FUNÇÕES DE INICIALIZAÇÃO ==========

void initializeHardware() {
  Serial.println("Initializing hardware...");
  
  // Configurar pinos
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  
  // Estado inicial OFF
  setDeviceState(false);
  
  Serial.printf("Hardware initialized:\n");
  Serial.printf("- LED Pin: %d\n", LED_PIN);
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
  }
  
  Serial.println("\nWiFi connected!");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("Signal strength: %d dBm\n", WiFi.RSSI());
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
  Serial.println("    HTTP Controlled Endpoint");
  Serial.println("========================================");
  
  initializeHardware();
  initializeWiFi();
  initializeWebServer();
  
  Serial.println("\n*** DEVICE READY ***");
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
  
  delay(10); // Pequeno delay para estabilidade
}

/*
 * ========== INSTRUÇÕES DE USO ==========
 * 
 * 1. CONFIGURAÇÃO:
 *    - Altere o local_IP para cada ESP32 (100, 101, 102, etc.)
 *    - Configure o mesmo SSID/password do bridge
 *    - Ajuste os pinos conforme seu hardware
 * 
 * 2. COMANDOS HTTP:
 *    GET /control?state=on   - Liga o dispositivo
 *    GET /control?state=off  - Desliga o dispositivo
 *    GET /status             - Verifica status atual
 * 
 * 3. INTEGRAÇÃO COM BRIDGE:
 *    - O bridge enviará comandos para http://IP:80/control
 *    - O bridge verificará status em http://IP:80/status
 * 
 * 4. EXPANSÕES POSSÍVEIS:
 *    - Adicionar sensores (temperatura, umidade, etc.)
 *    - Implementar controle PWM para dimmer
 *    - Adicionar autenticação HTTP
 *    - Implementar HTTPS para segurança
 * 
 */
