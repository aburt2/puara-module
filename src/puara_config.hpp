#pragma once
#include <string>

namespace PuaraImpl {
enum Monitors { UART_MONITOR = 0, JTAG_MONITOR = 1, USB_MONITOR = 2 };


  std::string get_dmi_name();
  unsigned int get_version();
  void set_version(unsigned int user_version);
  std::string getIP1();
  std::string getIP2();  
  bool IP1_ready();
  bool IP2_ready();
  int unsigned getPORT1();
  int unsigned getPORT2();
  std::string getPORT1Str();
  std::string getPORT2Str();
  int unsigned getLocalPORT();
  std::string getLocalPORTStr();

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