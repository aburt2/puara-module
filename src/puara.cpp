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

#include "puara_config.hpp"
#include "puara_device.hpp"
#include "puara_mdns.hpp"
#include "puara_serial.hpp"
#include "puara_spiffs.hpp"
#include "puara_web.hpp"
#include "puara_wifi.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <puara.h>

#include <iostream>
#include <optional>

struct PuaraGlobal
{
  PuaraAPI::DeviceConfiguration config;
  PuaraAPI::Device device;
  PuaraAPI::SPIFFS spiffs;
  PuaraAPI::JSONSettings settings{config, spiffs};
  PuaraAPI::Serial serial{config, device, spiffs, settings};
  PuaraAPI::WiFi wifi{config};
  PuaraAPI::Webserver webserver{config, device, spiffs, settings, wifi};
  PuaraAPI::MDNSService mdns;

  PuaraGlobal() { }

  void start(PuaraAPI::Monitors monitor)
  {
    std::cout << "\n"
              << "**********************************************************\n"
              << "* Puara Module Manager                                   *\n"
              << "* Metalab - Société des Arts Technologiques (SAT)        *\n"
              << "* Input Devices and Music Interaction Laboratory (IDMIL) *\n"
              << "* Edu Meneses (2022) - https://www.edumeneses.com        *\n"
              << "* Firmware version: " << config.version
              << "                             *\n"
              << "**********************************************************\n"
              << std::endl;

    spiffs.config_spiffs();
    settings.read_config_json();
    settings.read_settings_json();
    wifi.start_wifi();
    webserver.start_webserver();
    mdns.start(config.dmiName, config.dmiName);
    wifi.wifi_scan();

    serial.module_monitor = monitor;

    // some delay added as start listening blocks the hw monitor
    // std::cout << "Starting serial monitor..." << std::endl;
    // vTaskDelay(50 / portTICK_RATE_MS);
    // if(serial.start_serial_listening())
    // {
    // };
    // vTaskDelay(50 / portTICK_RATE_MS);
    // std::cout << "serial listening ready" << std::endl;

    std::cout << "Puara Start Done!\n\n  Type \"reboot\" in the serial monitor to reset "
                 "the ESP32.\n\n";
  }
};

static PuaraGlobal g_puara;

// Defining static members
void Puara::start(PuaraAPI::Monitors monitor)
{
  g_puara.start(monitor);
}

httpd_handle_t Puara::start_webserver(void)
{
  return g_puara.webserver.start_webserver();
}
void Puara::stop_webserver(void)
{
  return g_puara.webserver.stop_webserver();
}

std::string Puara::dmi_name()
{
  return g_puara.config.dmiName;
}
unsigned int Puara::version()
{
  return g_puara.config.version;
}
void Puara::set_version(unsigned int user_version)
{
  g_puara.config.version = user_version;
}
std::string Puara::IP1()
{
  return g_puara.config.oscIP1;
}
std::string Puara::IP2()
{
  return g_puara.config.oscIP2;
}
int unsigned Puara::PORT1()
{
  return g_puara.config.oscPORT1;
}
int unsigned Puara::PORT2()
{
  return g_puara.config.oscPORT2;
}
std::string Puara::PORT1Str()
{
  return std::to_string(g_puara.config.oscPORT1);
}
std::string Puara::PORT2Str()
{
  return std::to_string(g_puara.config.oscPORT2);
}
int unsigned Puara::LocalPORT()
{
  return g_puara.config.localPORT;
}
std::string Puara::LocalPORTStr()
{
  return std::to_string(g_puara.config.localPORT);
}
std::string Puara::staIP()
{
  return g_puara.wifi.currentSTA_IP;
}
std::string Puara::apIP()
{
  return g_puara.wifi.currentAP_IP;
}
unsigned int Puara::id()
{
  return g_puara.config.id;
}

bool Puara::persistentAP()
{
  return g_puara.config.persistentAP;
}
bool Puara::set_persistentAP(bool uservalue)
{
  g_puara.config.persistentAP = uservalue;

  return 1;
}

bool Puara::IP1_ready()
{
  return g_puara.config.IP1_ready();
}
bool Puara::IP2_ready()
{
  return g_puara.config.IP2_ready();
}


void Puara::config_spiffs()
{
  return g_puara.spiffs.config_spiffs();
}
void Puara::mount_spiffs()
{
  return g_puara.spiffs.mount_spiffs();
}
void Puara::unmount_spiffs()
{
  return g_puara.spiffs.unmount_spiffs();
}

void Puara::read_config_json()
{
  return g_puara.settings.read_config_json();
}
void Puara::write_config_json()
{
  return g_puara.settings.write_config_json();
}
void Puara::read_settings_json()
{
  return g_puara.settings.read_settings_json();
}
void Puara::write_settings_json()
{
  return g_puara.settings.write_settings_json();
}

double Puara::getVarNumber(std::string varName)
{
  return g_puara.settings.getVarNumber(varName);
}
std::string Puara::getVarText(std::string varName)
{
  return g_puara.settings.getVarText(varName);
}

bool Puara::start_serial_listening()
{
  return g_puara.serial.start_serial_listening();
}
void Puara::send_serial_data(std::string data)
{
  return g_puara.serial.send_serial_data(data);
}

void Puara::start_mdns_service(
    std::string_view device_name, std::string_view instance_name)
{
  return g_puara.mdns.start(device_name, instance_name);
}

void Puara::start_wifi()
{
  return g_puara.wifi.start_wifi();
}
void Puara::wifi_scan(void)
{
  return g_puara.wifi.wifi_scan();
}
bool Puara::get_StaIsConnected()
{
  return g_puara.wifi.get_StaIsConnected();
}
