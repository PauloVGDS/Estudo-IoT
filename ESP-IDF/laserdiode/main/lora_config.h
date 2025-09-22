#ifndef LORA_CONFIG_H
#define LORA_CONFIG_H

// Definições dos pinos SPI para o LoRa
#define LORA_SCK_PIN     18
#define LORA_MISO_PIN    19
#define LORA_MOSI_PIN    23
#define LORA_CS_PIN      5
#define LORA_RST_PIN     14
#define LORA_DIO0_PIN    2

// Configurações do LoRa
#define LORA_FREQUENCY   915E6  // 915 MHz
#define LORA_BANDWIDTH   125E3  // 125 kHz
#define LORA_SPREADING_FACTOR 7
#define LORA_CODING_RATE 5

#endif // LORA_CONFIG_H 