/*
 * ESP RainMaker + Matter Bridge Integration
 * 
 * Este código permite que o ESP32 apareça TANTO no ESP RainMaker
 * quanto seja controlado pelo Matter Bridge via HTTP
 * 
 * Hardware: ESP32 (ESP8266 não suporta RainMaker nativamente)
 * IDE: Arduino IDE
 * Apps: ESP RainMaker + Matter Apps
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "RMaker.h"
#include "WiFiProv.h"
#include "AppInsights.h"

// ========== CONFIGURAÇÕES DE REDE ==========
const char* ssid = "your-ssid";          
const char* password = "your-password";  

// ========== CONFIGURAÇÕES RAINMAKER ==========
#define DEFAULT_POWER_MODE true
const char *service_name = "PROV_ESP32";
const char *pop = "abcd1234";

// ========== HARDWARE ==========
const uint8_t LED_PIN = 2;
const uint8_t RELAY_PIN = 5;
static uint8_t gpio_reset = 0;

// ========== RAINMAKER DEVICES ==========
static LightBulb my_light("Light");
static Switch my_switch("Switch");

// ========== HTTP SERVER ==========
WebServer httpServer(80);

// ========== VARIÁVEIS GLOBAIS ==========
bool lightState = false;
bool switchState = false;
String deviceName = "ESP32-RainMaker-Device";

// ========== FUNÇÕES DE CONTROLE ==========

void updateHardware() {
  // Atualizar hardware baseado nos estados
  digitalWrite(LED_PIN, lightState ? HIGH : LOW);
  digitalWrite(RELAY_PIN, switchState ? HIGH : LOW);
  
  Serial.printf("Hardware updated - Light: %s, Switch: %s\n", 
                lightState ? "ON" : "OFF", 
                switchState ? "ON" : "OFF");
}

// ========== CALLBACKS RAINMAKER ==========

void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx) {
  const char *device_name = device->getDeviceName();
  const char *param_name = param->getParamName();

  if (strcmp(device_name, "Light") == 0) {
    if (strcmp(param_name, "Power") == 0) {
      lightState = val.val.b;
      Serial.printf("RainMaker Light state changed to %s\n", lightState ? "ON" : "OFF");
      updateHardware();
    }
  } else if (strcmp(device_name, "Switch") == 0) {
    if (strcmp(param_name, "Power") == 0) {
      switchState = val.val.b;
      Serial.printf("RainMaker Switch state changed to %s\n", switchState ? "ON" : "OFF");
      updateHardware();
    }
  }
}

// ========== HTTP HANDLERS (Para Matter Bridge) ==========

void handleControl() {
  String device = httpServer.arg("device");  // "light" ou "switch"
  String state = httpServer.arg("state");    // "on" ou "off"
  
  if (device == "" || state == "") {
    httpServer.send(400, "text/plain", "Missing parameters: device, state");
    return;
  }
  
  bool newState = (state == "on" || state == "true" || state == "1");
  
  // Atualizar dispositivo específico
  if (device == "light") {
    lightState = newState;
    my_light.updateAndReportParam("Power", lightState);
  } else if (device == "switch") {
    switchState = newState;
    my_switch.updateAndReportParam("Power", switchState);
  } else {
    httpServer.send(400, "text/plain", "Invalid device. Use 'light' or 'switch'");
    return;
  }
  
  updateHardware();
  
  // Resposta JSON
  StaticJsonDocument<200> response;
  response["status"] = "OK";
  response["device"] = device;
  response["state"] = newState ? "on" : "off";
  response["rainmaker_synced"] = true;
  
  String responseStr;
  serializeJson(response, responseStr);
  
  httpServer.send(200, "application/json", responseStr);
  
  Serial.printf("HTTP Control - %s: %s\n", device.c_str(), newState ? "ON" : "OFF");
}

void handleStatus() {
  StaticJsonDocument<400> status;
  
  status["device_name"] = deviceName;
  status["light_state"] = lightState ? "on" : "off";
  status["switch_state"] = switchState ? "on" : "off";
  status["rainmaker_connected"] = RMaker.isConnected();
  status["wifi_connected"] = WiFi.status() == WL_CONNECTED;
  status["uptime"] = millis();
  status["free_heap"] = ESP.getFreeHeap();
  status["ip_address"] = WiFi.localIP().toString();
  
  String statusStr;
  serializeJson(status, statusStr);
  
  httpServer.send(200, "application/json", statusStr);
}

void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP RainMaker + Matter Bridge</title>
    <style>
        body { font-family: Arial; text-align: center; margin: 50px; }
        .device { border: 1px solid #ccc; margin: 20px; padding: 20px; }
        .button { padding: 15px 30px; margin: 10px; font-size: 18px; }
        .on { background: #4CAF50; color: white; }
        .off { background: #f44336; color: white; }
    </style>
</head>
<body>
    <h1>ESP RainMaker + Matter Bridge Device</h1>
    
    <div class="device">
        <h3>Light Control</h3>
        <p>State: <strong id="light-state">)" + String(lightState ? "ON" : "OFF") + R"(</strong></p>
        <button class="button on" onclick="control('light', 'on')">Light ON</button>
        <button class="button off" onclick="control('light', 'off')">Light OFF</button>
    </div>
    
    <div class="device">
        <h3>Switch Control</h3>
        <p>State: <strong id="switch-state">)" + String(switchState ? "ON" : "OFF") + R"(</strong></p>
        <button class="button on" onclick="control('switch', 'on')">Switch ON</button>
        <button class="button off" onclick="control('switch', 'off')">Switch OFF</button>
    </div>
    
    <div class="device">
        <h3>Status</h3>
        <p>RainMaker Connected: <span id="rainmaker-status">)" + String(RMaker.isConnected() ? "YES" : "NO") + R"(</span></p>
        <p>IP: )" + WiFi.localIP().toString() + R"(</p>
    </div>
    
    <script>
        function control(device, state) {
            fetch('/control?device=' + device + '&state=' + state)
                .then(response => response.json())
                .then(data => {
                    document.getElementById(device + '-state').textContent = data.state.toUpperCase();
                    alert(device + ' turned ' + data.state.toUpperCase());
                })
                .catch(error => alert('Error: ' + error));
        }
        
        setInterval(() => {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('light-state').textContent = data.light_state.toUpperCase();
                    document.getElementById('switch-state').textContent = data.switch_state.toUpperCase();
                    document.getElementById('rainmaker-status').textContent = data.rainmaker_connected ? 'YES' : 'NO';
                });
        }, 5000);
    </script>
</body>
</html>
)";
  
  httpServer.send(200, "text/html", html);
}

// ========== INICIALIZAÇÃO ==========

void initializeRainMaker() {
  Serial.println("Initializing ESP RainMaker...");
  
  // Initialize the ESP RainMaker Agent
  RMaker.initTime(TZ_Asia_Kolkata);
  RMaker.initNode("ESP RainMaker Node");

  // Create and add devices
  my_light.addCb(write_callback);
  my_switch.addCb(write_callback);

  RMaker.addDevice(my_light);
  RMaker.addDevice(my_switch);

  // Enable OTA
  RMaker.enableOTA(OTA_USING_PARAMS);
  
  // Enable timezone service
  RMaker.enableTZService();
  
  // Enable scheduling
  RMaker.enableSchedule();

  Serial.printf("Starting ESP RainMaker with service name: %s\n", service_name);
  RMaker.start();

  // Initialize WiFi with provisioning
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, 
                         WIFI_PROV_SECURITY_1, pop, service_name);
}

void initializeHardware() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(gpio_reset, INPUT);
  
  updateHardware();
  
  Serial.printf("Hardware initialized - LED: %d, Relay: %d\n", LED_PIN, RELAY_PIN);
}

void initializeHTTPServer() {
  httpServer.on("/", handleRoot);
  httpServer.on("/control", handleControl);
  httpServer.on("/status", handleStatus);
  
  httpServer.onNotFound([]() {
    httpServer.send(404, "text/plain", "Endpoint not found");
  });
  
  httpServer.begin();
  Serial.println("HTTP Server started for Matter Bridge integration");
}

// ========== MAIN ==========

void setup() {
  Serial.begin(115200);
  
  Serial.println("========================================");
  Serial.println("  ESP RainMaker + Matter Bridge Device");
  Serial.println("========================================");
  
  initializeHardware();
  initializeRainMaker();
  initializeHTTPServer();
  
  Serial.println("\n*** DEVICE READY ***");
  Serial.println("1. Use ESP RainMaker app to control via cloud");
  Serial.println("2. Matter Bridge can control via HTTP");
  Serial.printf("3. Web interface: http://%s\n", WiFi.localIP().toString().c_str());
}

void loop() {
  // Handle HTTP requests for Matter Bridge
  httpServer.handleClient();
  
  // Handle reset button for RainMaker
  if (digitalRead(gpio_reset) == LOW) {
    Serial.println("Reset Button Pressed!");
    delay(100);
    int startTime = millis();
    while (digitalRead(gpio_reset) == LOW) delay(50);
    int endTime = millis();

    if ((endTime - startTime) > 10000) {
      Serial.println("Factory Reset!");
      RMakerFactoryReset(2);
    } else if ((endTime - startTime) > 3000) {
      Serial.println("Wi-Fi Reset!");
      RMakerWiFiReset(2);
    }
  }
  
  delay(100);
}

/*
 * ========== COMO FUNCIONA ==========
 * 
 * 1. RAINMAKER INTEGRATION:
 *    - Dispositivo aparece no app ESP RainMaker
 *    - 2 devices: Light e Switch
 *    - Controle via cloud/app
 * 
 * 2. MATTER BRIDGE INTEGRATION:
 *    - HTTP API para controle local
 *    - URLs: /control?device=light&state=on
 *    - Estados sincronizados entre RainMaker e Matter
 * 
 * 3. DUAL CONTROL:
 *    - RainMaker app → Callback → Hardware
 *    - Matter Bridge → HTTP → RainMaker → Hardware
 *    - Estados sempre sincronizados
 * 
 * ========== CONFIGURAÇÃO ==========
 * 
 * 1. Instalar ESP RainMaker Library
 * 2. Baixar ESP RainMaker app
 * 3. Usar QR code para provisioning
 * 4. Configurar Matter Bridge com IPs dos ESP32s
 * 
 */
