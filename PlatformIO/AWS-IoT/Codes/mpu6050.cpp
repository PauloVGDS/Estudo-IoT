// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup(void)
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10); // will pause Zero, Leonardo, etc until serial console opens
    }

    // Try to initialize!
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }

    Serial.println("");
    delay(100);
}

void loop()
{

    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Serial.print("Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degC");
    
    

    delay(10);
}