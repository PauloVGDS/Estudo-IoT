#ifndef LORA_REGISTERS_H
#define LORA_REGISTERS_H

// Registradores do LoRa
#define REG_FIFO                 0x00
#define REG_OP_MODE             0x01
#define REG_FR_MSB              0x06
#define REG_FR_MID              0x07
#define REG_FR_LSB              0x08
#define REG_PA_CONFIG           0x09
#define REG_PA_RAMP             0x0A
#define REG_OCP                 0x0B
#define REG_LNA                 0x0C
#define REG_FIFO_ADDR_PTR      0x0D
#define REG_FIFO_TX_BASE       0x0E
#define REG_FIFO_RX_BASE       0x0F
#define REG_FIFO_RX_CURRENT    0x10
#define REG_IRQ_FLAGS          0x12
#define REG_RX_NB_BYTES        0x13
#define REG_PKT_SNR_VALUE      0x19
#define REG_PKT_RSSI_VALUE     0x1A
#define REG_MODEM_CONFIG_1     0x1D
#define REG_MODEM_CONFIG_2     0x1E
#define REG_PREAMBLE_MSB       0x20
#define REG_PREAMBLE_LSB       0x21
#define REG_PAYLOAD_LENGTH     0x22
#define REG_MODEM_CONFIG_3     0x26
#define REG_RSSI_WIDEBAND      0x2C
#define REG_DETECTION_OPTIMIZE  0x31
#define REG_DETECTION_THRESHOLD 0x37
#define REG_SYNC_WORD          0x39
#define REG_DIO_MAPPING_1      0x40
#define REG_VERSION            0x42

// Modos de operação
#define MODE_LONG_RANGE_MODE   0x80
#define MODE_SLEEP             0x00
#define MODE_STDBY             0x01
#define MODE_TX                0x03
#define MODE_RX_CONTINUOUS     0x05
#define MODE_RX_SINGLE         0x06

// Flags de IRQ
#define IRQ_TX_DONE_MASK       0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK  0x20
#define IRQ_RX_DONE_MASK       0x40

#endif // LORA_REGISTERS_H 