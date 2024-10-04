#pragma once

#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>

#include <string>

namespace PuaraImpl {
// Public API

void start_wifi();
void wifi_scan(void);
bool get_StaIsConnected();

// Private API

inline const int wifi_connected_bit = BIT0;
inline const int wifi_fail_bit = BIT1;
inline EventGroupHandle_t s_wifi_event_group;

inline wifi_config_t wifi_config_sta;
inline wifi_config_t wifi_config_ap;
inline short int connect_counter;
inline bool StaIsConnected = false;
inline bool ApStarted = false;

inline std::string currentSSID;
inline std::string currentSTA_IP;
inline std::string currentSTA_MAC;
inline std::string currentAP_IP;
inline std::string currentAP_MAC;

void sta_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void wifi_init();

}  // namespace PuaraImpl
