#include "certs.h"
#include "declare.h"

void setup()
{

    Serial.begin(115200);
    delay(50);
    ESP_LOGI(TAG, "\nESP32 AWS IoT Example\n");
    initDisplay();                  // Inicializa o display via I2C
    
    if (!mpu.begin())
    {
        ESP_LOGE(MPU_TAG, "MPU6050 Failed!\n");

        while (1)
        {
            delay(10);
        }
    }
    ESP_LOGI(MPU_TAG, "MPU6050 iniciated!\n");

    ESP_LOGI(TAG, "Connecting to ");
    ESP_LOGI(TAG, "%s\n", ssid);
    WiFi.begin(ssid, password);
    WiFi.waitForConnectResult();
    ESP_LOGI(TAG, "WiFi connected");
    ESP_LOGI(TAG, "IP address: %s\n", WiFi.localIP().toString().c_str());

    ESP_LOGI(TAG, "Device ID: %s\n", deviceID);
    sntp_stop(); 
    sntp_init(); 
    setCurrentTime();
    ESP_LOGI(SNTP_TAG, "Time set!\n");

    wiFiClient.setCACert(rootCA);
    wiFiClient.setCertificate(certificate_pem_crt);
    wiFiClient.setPrivateKey(private_pem_key);
    ESP_LOGI(MQTT_TAG, "Certificates loaded!\n");

    ESP_LOGI(TAG, "IP address: %s\n", WiFi.localIP().toString().c_str());


}

void loop()
{

    pubSubCheckConnect();
    displayText("Temperature:", 1, false, false);
    if (millis() - lastPublish > 5000)
    {

        // Lê a temperatura em Celsius
        mpu.getEvent(&a, &g, &temp);

        ESP_LOGI(MPU_TAG, "Temperature: %.2f degC", temp.temperature);

        displayText(String(temp.temperature), 2, true, true);
        // Cria um objeto JSON
        JsonDocument doc;
        doc["Device"] = deviceID;
        doc["Temperatura"] = temp.temperature;
        doc["Model"] = "ESP32";
        doc["Type"] = "Sensor";

        // Serializa para string
        String jsonMsg;
        serializeJson(doc, jsonMsg);

        // Publica a mensagem JSON
        boolean rc = pubSubClient.publish(topic, jsonMsg.c_str());
        ESP_LOGI(MQTT_TAG, "Published, rc= %s", (rc ? "OK" : "FAILED"));

        ESP_LOGI(MQTT_TAG, "%s\n", jsonMsg.c_str());
        lastPublish = millis();
    }
}

void msgReceived(char *topic, byte *payload, unsigned int length)
{
    String message;
    ESP_LOGI(MQTT_TAG, "Message received on ");
    ESP_LOGI(MQTT_TAG, "%s: ", topic);
    for (int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }
    ESP_LOGI(MQTT_TAG, "%s", message.c_str());
    ESP_LOGI(MQTT_TAG, "\n");
}

void pubSubCheckConnect()
{
    if (!pubSubClient.connected())
    {
        ESP_LOGI(MQTT_TAG, "PubSubClient connecting to: %s", awsEndpoint);

        while (!pubSubClient.connected())
        {
            pubSubClient.connect("ESPthing"); // Conecta com o ID `ESPthing`
            delay(1000);
        }
        ESP_LOGI(MQTT_TAG, "Connected\n");
        pubSubClient.subscribe("inTopic"); // Inscrito no tópico "inTopic"
    }
    pubSubClient.loop();
}

void setCurrentTime()
{
    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    ESP_LOGI(SNTP_TAG, "Waiting for NTP time sync: ");

    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2)
    {
        delay(500);

        now = time(nullptr);
    }

    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    ESP_LOGI(SNTP_TAG, "Current time: %s", asctime(&timeinfo));
}

void initDisplay()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        ESP_LOGE(DISPLAY_TAG, "Falha na inicialização do SSD1306 via I2C");

        for (;;)
            ; // Loop infinito se falhar
    }
    display.clearDisplay();             // Posição inicial
    display.display();                   // Atualiza o display
    ESP_LOGI(DISPLAY_TAG, "Display initialized!\n");

}

void displayText(String text, int textSize, bool center, bool end) {

    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

    int x = (SCREEN_WIDTH - w) / 2;
    int y = (SCREEN_HEIGHT - h) / 2;

    if (center) {

        display.setCursor(x, y);
    } else {
        display.setCursor(x, 0);
    }

    display.println(text);

    if (end) {
        display.display();
        delay(1000);
        display.clearDisplay();
    }
}
