#include <Wire.h> // Para comunicação I2C
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128     // Largura do display (ajuste para 128 se for 128x32/64)
#define SCREEN_HEIGHT 64    // Altura do display (ajuste para 64 se for 128x64)
#define OLED_RESET -1       // Pino de reset (ou -1 se compartilhado)
#define SCREEN_ADDRESS 0x3C // Endereço I2C padrão para SSD1306
int counter = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Função para inicializar o display via I2C
void initDisplay()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("Falha na inicialização do SSD1306 via I2C"));
        for (;;)
            ; // Loop infinito se falhar
    }
    display.clearDisplay();             // Posição inicial
    display.display();                   // Atualiza o display
}

void displayText(String text, int textSize = 1, bool center = false, bool end = false) {

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


void setup()
{
    Serial.begin(115200);
    initDisplay();                  // Inicializa o display via I2C
}

void loop()
{
    // Aqui você pode chamar displayText() com dados dinâmicos, ex.:
    // displayText("Temp: " + String(temp.temperature));
    // displayText("Temp: " + counter);
    // counter++;
    
    
    displayText("Temperature:", 1);
    displayText("30C", 2, true, true);
}