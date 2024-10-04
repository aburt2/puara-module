/*

Puara Module Manager
Metalab - Société des Arts Technologiques (SAT)
Input Devices and Music Interaction Laboratory (IDMIL), McGill University
Edu Meneses (2022) - https://www.edumeneses.com

- event_handler, wifi_init_sta, and start_wifi were modified from
  https://github.com/espressif/esp-idf/tree/master/examples/wifi/getting_started/station
- mount_spiffs, and unmount_spiffs were modified from
  https://github.com/espressif/esp-idf/tree/master/examples/storage

*/

#include <puara.h>

// Defining static members
void Puara::start(Monitors monitor) {
  std::cout << "\n"
            << "**********************************************************\n"
            << "* Puara Module Manager                                   *\n"
            << "* Metalab - Société des Arts Technologiques (SAT)        *\n"
            << "* Input Devices and Music Interaction Laboratory (IDMIL) *\n"
            << "* Edu Meneses (2022) - https://www.edumeneses.com        *\n"
            << "* Firmware version: " << version << "                             *\n"
            << "**********************************************************\n"
            << std::endl;

  config_spiffs();
  read_config_json();
  read_settings_json();
  start_wifi();
  start_webserver();
  start_mdns_service(dmiName, dmiName);
  wifi_scan();

  module_monitor = monitor;

  // some delay added as start listening blocks the hw monitor
  std::cout << "Starting serial monitor..." << std::endl;
  vTaskDelay(50 / portTICK_RATE_MS);
  if (start_serial_listening()) {
  };
  vTaskDelay(50 / portTICK_RATE_MS);
  std::cout << "serial listening ready" << std::endl;

  std::cout
      << "Puara Start Done!\n\n  Type \"reboot\" in the serial monitor to reset the ESP32.\n\n";
}

