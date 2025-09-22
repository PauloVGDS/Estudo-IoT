#include "string.h"
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_partition.h>
#include <esp_ota_ops.h>

#define TAG "INTEGRITY_CHECK"
#define HASH_LEN 32  // SHA-256 produces a 32-byte hash

static bool verify_sha256(const uint8_t *calculated_hash, const uint8_t *expected_hash) {
    for (int i = 0; i < HASH_LEN; i++) {
        if (calculated_hash[i] != expected_hash[i]) {
            return false;
        }
    }
    return true;
}

static void print_sha256(const uint8_t *image_hash, const char *label) {
    char hash_print[HASH_LEN * 2 + 1];
    hash_print[HASH_LEN * 2] = 0;
    for (int i = 0; i < HASH_LEN; ++i) {
        sprintf(&hash_print[i * 2], "%02x", image_hash[i]);
    }
    ESP_LOGI(TAG, "%s %s", label, hash_print);
}

static void calculate_and_print_sha256(const esp_partition_t *partition, const char *label, const uint8_t *expected_hash) {
    uint8_t sha_256[HASH_LEN] = { 0 };
    esp_err_t err = esp_partition_get_sha256(partition, sha_256);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SHA-256 for %s: %s", label, esp_err_to_name(err));
        return;
    }
    print_sha256(sha_256, label);

    if (expected_hash) {
        if (verify_sha256(sha_256, expected_hash)) {
            ESP_LOGI(TAG, "%s integrity verified", label);
        } else {
            ESP_LOGE(TAG, "%s integrity check failed!", label);
        }
    }
}

static void get_sha256_of_partitions(void) {
    // Valores dos Hashes esperados
    // Descobrir forma de obter os valores externamente
    uint8_t expected_bootloader_hash[HASH_LEN] = { /* Hash do bootloader */ };
    uint8_t expected_firmware_hash[HASH_LEN] = { /* Hash do firmware*/ };

    // Bootloader partition
    const esp_partition_t *bootloader_partition = esp_partition_find_first(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
    if (bootloader_partition) {
        calculate_and_print_sha256(bootloader_partition, "SHA-256 for bootloader: ", expected_bootloader_hash);
    } else {
        ESP_LOGE(TAG, "Bootloader partition not found");
    }

    // Running partition
    const esp_partition_t *running_partition = esp_ota_get_running_partition();
    if (running_partition) {
        calculate_and_print_sha256(running_partition, "SHA-256 for current firmware: ", expected_firmware_hash);
    } else {
        ESP_LOGE(TAG, "Running partition not found");
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Starting");

    get_sha256_of_partitions();

    ESP_LOGI(TAG, "Finished");
}
