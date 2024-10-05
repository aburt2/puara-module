#include "puara_device.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace PuaraAPI
{
static const int reboot_delay = 3000;

void Device::reboot_with_delay()
{
  vTaskDelay(reboot_delay / portTICK_RATE_MS);
  esp_restart();
}

} 