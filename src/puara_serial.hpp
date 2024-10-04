#pragma once
#include "puara_config.hpp"

namespace PuaraImpl {
// Set default monitor as UART
inline int module_monitor = UART_MONITOR;

void interpret_serial(void* pvParameters);
void uart_monitor(void* pvParameters);
void jtag_monitor(void* pvParameters);
void usb_monitor(void* pvParameters);
}  // namespace PuaraImpl