#pragma once
#include <string>

namespace PuaraImpl {
enum Monitors { UART_MONITOR = 0, JTAG_MONITOR = 1, USB_MONITOR = 2 };

inline unsigned int version = 20220906;
inline std::string dmiName;
inline std::string device;
inline unsigned int id;
inline std::string author;
inline std::string institution;
inline std::string APpasswd;
inline std::string wifiSSID;
inline std::string wifiPSK;
inline bool persistentAP = false;
inline std::string oscIP1;
inline unsigned int oscPORT1;
inline std::string oscIP2;
inline unsigned int oscPORT2;
inline unsigned int localPORT;
inline std::string wifiAvailableSsid;

}  // namespace PuaraImpl