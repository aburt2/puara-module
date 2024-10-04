#include "puara.h"

unsigned int Puara::version = 20220906;
std::string Puara::dmiName;
std::string Puara::device;
unsigned int Puara::id;
std::string Puara::author;
std::string Puara::institution;
std::string Puara::APpasswd;
std::string Puara::wifiSSID;
std::string Puara::wifiPSK;
bool Puara::persistentAP = false;
std::string Puara::oscIP1;
unsigned int Puara::oscPORT1;
std::string Puara::oscIP2;
unsigned int Puara::oscPORT2;
unsigned int Puara::localPORT;
std::string Puara::wifiAvailableSsid;

unsigned int Puara::get_version() { return version; };

void Puara::set_version(unsigned int user_version) { version = user_version; };

std::string Puara::get_dmi_name() { return dmiName; }

std::string Puara::getIP1() { return oscIP1; }

std::string Puara::getIP2() { return oscIP2; }

int unsigned Puara::getPORT1() { return oscPORT1; }

int unsigned Puara::getPORT2() { return oscPORT2; }

std::string Puara::getPORT1Str() { return std::to_string(oscPORT1); }

std::string Puara::getPORT2Str() { return std::to_string(oscPORT2); }

int unsigned Puara::getLocalPORT() { return localPORT; }

std::string Puara::getLocalPORTStr() { return std::to_string(localPORT); }

bool Puara::IP1_ready() {
  if ((oscIP1 == "0.0.0.0") || (oscIP1 == "")) {
    return false;
  } else {
    return true;
  }
}


bool Puara::IP2_ready() {
  if ((oscIP2 == "0.0.0.0") || (oscIP2 == "")) {
    return false;
  } else {
    return true;
  }
}
