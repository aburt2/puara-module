#include <mdns.h>

#include <iostream>

#include "puara.h"

void Puara::start_mdns_service(std::string_view device_name, std::string_view instance_name) {
  // initialize mDNS service
  esp_err_t err = mdns_init();
  if (err) {
    std::cout << "MDNS Init failed: " << err << std::endl;
    return;
  }
  // set hostname
  ESP_ERROR_CHECK(mdns_hostname_set(device_name.data()));
  // set default instance
  ESP_ERROR_CHECK(mdns_instance_name_set(instance_name.data()));
  std::cout << "MDNS Init completed. Device name: " << device_name << "\n" << std::endl;
}