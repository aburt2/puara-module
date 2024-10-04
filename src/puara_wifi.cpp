#include "puara_wifi.hpp"

#include <nvs_flash.h>

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "puara_config.hpp"

namespace PuaraImpl {
static const short int channel = 6;
static const short int max_connection = 5;
static const short int wifi_maximum_retry = 5;
static const int wifiScanSize = 20;
}  // namespace PuaraImpl

void PuaraImpl::wifi_init() {
  s_wifi_event_group = xEventGroupCreate();

  ESP_ERROR_CHECK(esp_netif_init());

  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();
  esp_netif_t* ap_netif = esp_netif_create_default_wifi_ap();  // saving pointer to
                                                               // retrieve AP ip later

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  // Set device hostname
  esp_err_t setname = tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, dmiName.c_str());
  if (setname != ESP_OK) {
    std::cout << "wifi_init: failed to set hostname: " << dmiName << std::endl;
  } else {
    std::cout << "wifi_init: hostname: " << dmiName << std::endl;
  }

  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &PuaraImpl::sta_event_handler, NULL, &instance_any_id));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &PuaraImpl::sta_event_handler, NULL, &instance_got_ip));

  std::cout << "wifi_init: setting wifi mode" << std::endl;
  if (persistentAP) {
    std::cout << "wifi_init:     AP-STA mode" << std::endl;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    std::cout << "wifi_init: loading AP config" << std::endl;
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config_ap));
  } else {
    std::cout << "wifi_init:     STA mode" << std::endl;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  }
  std::cout << "wifi_init: loading STA config" << std::endl;
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config_sta));
  std::cout << "wifi_init: esp_wifi_start" << std::endl;
  ESP_ERROR_CHECK(esp_wifi_start());

  std::cout << "wifi_init: wifi_init finished." << std::endl;

  /* Waiting until either the connection is established (PuaraImpl::wifi_connected_bit)
   * or connection failed for the maximum number of re-tries (PuaraImpl::wifi_fail_bit).
   * The bits are set by event_handler() (see above) */
  EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                         PuaraImpl::wifi_connected_bit | PuaraImpl::wifi_fail_bit,
                                         pdFALSE,
                                         pdFALSE,
                                         portMAX_DELAY);

  /* xEventGroupWaitBits() returns the bits before the call returned, hence we
   * can test which event actually happened. */
  if (bits & PuaraImpl::wifi_connected_bit) {
    std::cout << "wifi_init: Connected to SSID: " << PuaraImpl::wifiSSID << std::endl;
    currentSSID = wifiSSID;
    PuaraImpl::StaIsConnected = true;
  } else if (bits & PuaraImpl::wifi_fail_bit) {
    std::cout << "wifi_init: Failed to connect to SSID: " << PuaraImpl::wifiSSID << std::endl;
    if (!persistentAP) {
      std::cout << "wifi_init: Failed to connect to SSID: " << PuaraImpl::wifiSSID
                << "Switching to AP/STA mode" << std::endl;
      ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
      std::cout << "wifi_init: loading AP config" << std::endl;
      ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config_ap));
      std::cout << "wifi_init: Trying to connect one more time to SSID before giving up."
                << std::endl;
      ESP_ERROR_CHECK(esp_wifi_start());
    } else {
      PuaraImpl::StaIsConnected = false;
    }
  } else {
    std::cout << "wifi_init: UNEXPECTED EVENT" << std::endl;
  }

  /* The event will not be processed after unregister */
  ESP_ERROR_CHECK(
      esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
  ESP_ERROR_CHECK(
      esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
  vEventGroupDelete(s_wifi_event_group);

  // getting extra info
  unsigned char temp_info[6] = {0};
  esp_wifi_get_mac(WIFI_IF_STA, temp_info);
  std::ostringstream tempBuf;
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[0] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[1] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[2] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[3] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[4] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[5];
  PuaraImpl::currentSTA_MAC = tempBuf.str();
  tempBuf.clear();             // preparing the ostringstream
  tempBuf.str(std::string());  // buffer for reuse
  esp_wifi_get_mac(WIFI_IF_AP, temp_info);
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[0] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[1] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[2] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[3] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[4] << ":";
  tempBuf << std::setfill('0') << std::setw(2) << std::hex << (int)temp_info[5];
  PuaraImpl::currentAP_MAC = tempBuf.str();

  esp_netif_ip_info_t ip_temp_info;
  esp_netif_get_ip_info(ap_netif, &ip_temp_info);
  tempBuf.clear();
  tempBuf.str(std::string());
  tempBuf << std::dec << esp_ip4_addr1_16(&ip_temp_info.ip) << ".";
  tempBuf << std::dec << esp_ip4_addr2_16(&ip_temp_info.ip) << ".";
  tempBuf << std::dec << esp_ip4_addr3_16(&ip_temp_info.ip) << ".";
  tempBuf << std::dec << esp_ip4_addr4_16(&ip_temp_info.ip);
  PuaraImpl::currentAP_IP = tempBuf.str();
}

void PuaraImpl::wifi_scan(void) {
  uint16_t number = wifiScanSize;
  wifi_ap_record_t ap_info[wifiScanSize];
  uint16_t ap_count = 0;
  memset(ap_info, 0, sizeof(ap_info));

  esp_wifi_scan_start(NULL, true);
  ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
  ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
  std::cout << "wifi_scan: Total APs scanned = " << ap_count << std::endl;
  wifiAvailableSsid.clear();
  for (int i = 0; (i < wifiScanSize) && (i < ap_count); i++) {
    wifiAvailableSsid.append("<strong>SSID: </strong>");
    wifiAvailableSsid.append(reinterpret_cast<const char*>(ap_info[i].ssid));
    wifiAvailableSsid.append("<br>      (RSSI: ");
    wifiAvailableSsid.append(std::to_string(ap_info[i].rssi));
    wifiAvailableSsid.append(", Channel: ");
    wifiAvailableSsid.append(std::to_string(ap_info[i].primary));
    wifiAvailableSsid.append(")<br>");
  }
}

void PuaraImpl::start_wifi() {
  ApStarted = false;

  // Check if wifiSSID is empty and wifiPSK have less than 8 characteres
  if (dmiName.empty()) {
    std::cout << "start_wifi: Module name unpopulated. Using default name: Puara" << std::endl;
    dmiName = "Puara";
  }
  if (APpasswd.empty() || APpasswd.length() < 8 || APpasswd == "password") {
    std::cout << "startWifi: AP password error. Possible causes:" << "\n"
              << "startWifi:   - no AP password" << "\n"
              << "startWifi:   - password is less than 8 characteres long" << "\n"
              << "startWifi:   - password is set to \"password\"" << "\n"
              << "startWifi: Using default AP password: password" << "\n"
              << "startWifi: It is strongly recommended to change the password" << std::endl;
    APpasswd = "password";
  }
  if (wifiSSID.empty()) {
    std::cout << "start_wifi: No blank SSID allowed. Using default name: Puara" << std::endl;
    wifiSSID = "Puara";
  }

  strncpy((char*)PuaraImpl::wifi_config_sta.sta.ssid,
          PuaraImpl::wifiSSID.c_str(),
          PuaraImpl::wifiSSID.length() + 1);
  strncpy((char*)PuaraImpl::wifi_config_sta.sta.password,
          PuaraImpl::wifiPSK.c_str(),
          PuaraImpl::wifiPSK.length() + 1);
  strncpy((char*)PuaraImpl::wifi_config_ap.ap.ssid,
          PuaraImpl::dmiName.c_str(),
          PuaraImpl::dmiName.length() + 1);
  PuaraImpl::wifi_config_ap.ap.ssid_len = PuaraImpl::dmiName.length();
  PuaraImpl::wifi_config_ap.ap.channel = PuaraImpl::channel;
  strncpy((char*)PuaraImpl::wifi_config_ap.ap.password,
          PuaraImpl::APpasswd.c_str(),
          PuaraImpl::APpasswd.length() + 1);
  PuaraImpl::wifi_config_ap.ap.max_connection = PuaraImpl::max_connection;
  PuaraImpl::wifi_config_ap.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  std::cout << "startWifi: Starting WiFi config" << std::endl;
  PuaraImpl::connect_counter = 0;
  wifi_init();
  ApStarted = true;
}

void PuaraImpl::sta_event_handler(void* arg,
                                  esp_event_base_t event_base,
                                  int32_t event_id,
                                  void* event_data) {
  // int counter = 0;
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    printf("%d, %d", PuaraImpl::connect_counter, PuaraImpl::wifi_maximum_retry);
    if (PuaraImpl::connect_counter < PuaraImpl::wifi_maximum_retry) {
      PuaraImpl::connect_counter++;
      esp_wifi_connect();
      std::cout << "wifi/sta_event_handler: retry to connect to the AP" << std::endl;
    } else {
      xEventGroupSetBits(s_wifi_event_group, PuaraImpl::wifi_fail_bit);
    }
    std::cout << "wifi/sta_event_handler: connect to the AP fail" << std::endl;
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;

    std::stringstream tempBuf;
    tempBuf << esp_ip4_addr1_16(&event->ip_info.ip) << ".";
    tempBuf << esp_ip4_addr2_16(&event->ip_info.ip) << ".";
    tempBuf << esp_ip4_addr3_16(&event->ip_info.ip) << ".";
    tempBuf << esp_ip4_addr4_16(&event->ip_info.ip);
    PuaraImpl::currentSTA_IP = tempBuf.str();
    std::cout << "wifi/sta_event_handler: got ip:" << PuaraImpl::currentSTA_IP << std::endl;
    PuaraImpl::connect_counter = 0;
    xEventGroupSetBits(s_wifi_event_group, PuaraImpl::wifi_connected_bit);
  }
}

bool PuaraImpl::get_StaIsConnected() { return StaIsConnected; }
