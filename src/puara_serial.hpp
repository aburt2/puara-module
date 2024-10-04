#pragma once
#include "puara_config.hpp"

namespace PuaraImpl {
// Set default monitor as UART
inline int module_monitor = UART_MONITOR;

// public API

  bool start_serial_listening();
  void send_serial_data(std::string data);

// private API
void interpret_serial(void* pvParameters);
void uart_monitor(void* pvParameters);
void jtag_monitor(void* pvParameters);
void usb_monitor(void* pvParameters);
}  // namespace PuaraImpl