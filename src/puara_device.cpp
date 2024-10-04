#include "puara.h"

void Puara::reboot_with_delay(void* pvParameter) {
  vTaskDelay(reboot_delay / portTICK_RATE_MS);
  esp_restart();
}
