#include "puara_device.hpp"

#include "puara.h"

namespace Puara {
static const int reboot_delay = 3000;
}

void Puara::reboot_with_delay(void* pvParameter) {
  vTaskDelay(reboot_delay / portTICK_RATE_MS);
  esp_restart();
}
