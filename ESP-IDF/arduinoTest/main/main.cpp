#include "Arduino.h"

extern "C"
{
    void app_main();
}

extern "C" void app_main()
{
    // Initialize the Arduino framework
    initArduino();

    // Your main application code goes here
    Serial.begin(115200);
    Serial.println("Hello, Arduino on ESP32!");

    // Add your application logic here
    while (true)
    {
        Serial.println("Running...");
        delay(1000); // Delay for 1 second
    }
}