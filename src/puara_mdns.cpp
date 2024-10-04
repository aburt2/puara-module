#include "puara.h"

void Puara::start_mdns_service(const char* device_name, const char* instance_name) {
  // initialize mDNS service
  esp_err_t err = mdns_init();
  if (err) {
    std::cout << "MDNS Init failed: " << err << std::endl;
    return;
  }
  // set hostname
  ESP_ERROR_CHECK(mdns_hostname_set(device_name));
  // set default instance
  ESP_ERROR_CHECK(mdns_instance_name_set(instance_name));
  std::cout << "MDNS Init completed. Device name: " << device_name << "\n" << std::endl;
}

void Puara::start_mdns_service(std::string device_name, std::string instance_name) {
  // initialize mDNS service
  esp_err_t err = mdns_init();
  if (err) {
    std::cout << "MDNS Init failed: " << err << std::endl;
    return;
  }
  // set hostname
  ESP_ERROR_CHECK(mdns_hostname_set(device_name.c_str()));
  // set default instance
  ESP_ERROR_CHECK(mdns_instance_name_set(instance_name.c_str()));
  std::cout << "MDNS Init completed. Device name: " << device_name << "\n" << std::endl;
}
