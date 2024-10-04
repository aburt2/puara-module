#include "puara_device.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace PuaraImpl {
static const int reboot_delay = 3000;
}

void PuaraImpl::reboot_with_delay(void* pvParameter) {
  vTaskDelay(reboot_delay / portTICK_RATE_MS);
  esp_restart();
}
