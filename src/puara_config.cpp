#include "puara_config.hpp"

unsigned int PuaraImpl::get_version() { return version; };

void PuaraImpl::set_version(unsigned int user_version) { version = user_version; };

std::string PuaraImpl::get_dmi_name() { return dmiName; }

std::string PuaraImpl::getIP1() { return oscIP1; }

std::string PuaraImpl::getIP2() { return oscIP2; }

int unsigned PuaraImpl::getPORT1() { return oscPORT1; }

int unsigned PuaraImpl::getPORT2() { return oscPORT2; }

std::string PuaraImpl::getPORT1Str() { return std::to_string(oscPORT1); }

std::string PuaraImpl::getPORT2Str() { return std::to_string(oscPORT2); }

int unsigned PuaraImpl::getLocalPORT() { return localPORT; }

std::string PuaraImpl::getLocalPORTStr() { return std::to_string(localPORT); }

bool PuaraImpl::IP1_ready() {
  if ((oscIP1 == "0.0.0.0") || (oscIP1 == "")) {
    return false;
  } else {
    return true;
  }
}

bool PuaraImpl::IP2_ready() {
  if ((oscIP2 == "0.0.0.0") || (oscIP2 == "")) {
    return false;
  } else {
    return true;
  }
}
