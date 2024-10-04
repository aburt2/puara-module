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

#include <iostream>

#include "puara_config.hpp"
#include "puara_serial.hpp"
#include "puara_spiffs.hpp"
#include "puara_web.hpp"
#include "puara_wifi.hpp"
#include "puara_mdns.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Defining static members
void Puara::start(PuaraImpl::Monitors monitor) {
  std::cout << "\n"
            << "**********************************************************\n"
            << "* Puara Module Manager                                   *\n"
            << "* Metalab - Société des Arts Technologiques (SAT)        *\n"
            << "* Input Devices and Music Interaction Laboratory (IDMIL) *\n"
            << "* Edu Meneses (2022) - https://www.edumeneses.com        *\n"
            << "* Firmware version: " << PuaraImpl::version << "                             *\n"
            << "**********************************************************\n"
            << std::endl;

  PuaraImpl::config_spiffs();
  PuaraImpl::read_config_json();
  PuaraImpl::read_settings_json();
  PuaraImpl::start_wifi();
  PuaraImpl::start_webserver();
  PuaraImpl::start_mdns_service(PuaraImpl::dmiName, PuaraImpl::dmiName);
  PuaraImpl::wifi_scan();

  PuaraImpl::module_monitor = monitor;

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

httpd_handle_t Puara::start_webserver(void) { return PuaraImpl::start_webserver(); }
void Puara::stop_webserver(void) { return PuaraImpl::stop_webserver(); }

std::string Puara::get_dmi_name() { return PuaraImpl::get_dmi_name(); }
unsigned int Puara::get_version() { return PuaraImpl::get_version(); }
void Puara::set_version(unsigned int user_version) { return PuaraImpl::set_version(user_version); }
std::string Puara::getIP1() { return PuaraImpl::getIP1(); }
std::string Puara::getIP2() { return PuaraImpl::getIP2(); }
bool Puara::IP1_ready() { return PuaraImpl::IP1_ready(); }
bool Puara::IP2_ready() { return PuaraImpl::IP2_ready(); }
int unsigned Puara::getPORT1() { return PuaraImpl::getPORT1(); }
int unsigned Puara::getPORT2() { return PuaraImpl::getPORT2(); }
std::string Puara::getPORT1Str() { return PuaraImpl::getPORT1Str(); }
std::string Puara::getPORT2Str() { return PuaraImpl::getPORT2Str(); }
int unsigned Puara::getLocalPORT() { return PuaraImpl::getLocalPORT(); }
std::string Puara::getLocalPORTStr() { return PuaraImpl::getLocalPORTStr(); }

void Puara::config_spiffs() { return PuaraImpl::config_spiffs(); }
void Puara::mount_spiffs() { return PuaraImpl::mount_spiffs(); }
void Puara::unmount_spiffs() { return PuaraImpl::unmount_spiffs(); }

void Puara::read_config_json() { return PuaraImpl::read_config_json(); }
void Puara::write_config_json() { return PuaraImpl::write_config_json(); }
void Puara::read_settings_json() { return PuaraImpl::read_settings_json(); }
void Puara::write_settings_json() { return PuaraImpl::write_settings_json(); }

bool Puara::start_serial_listening() { return PuaraImpl::start_serial_listening(); }
void Puara::send_serial_data(std::string data) { return PuaraImpl::send_serial_data(data); }

void Puara::start_mdns_service(std::string_view device_name, std::string_view instance_name) {
  return PuaraImpl::start_mdns_service(device_name, instance_name);
}

void Puara::start_wifi() { return PuaraImpl::start_wifi(); }
void Puara::wifi_scan(void) { return PuaraImpl::wifi_scan(); }
bool Puara::get_StaIsConnected() { return PuaraImpl::get_StaIsConnected(); }

double Puara::getVarNumber(std::string varName) { return PuaraImpl::getVarNumber(varName); }
std::string Puara::getVarText(std::string varName) { return PuaraImpl::getVarText(varName); }
