#include "puara_config.hpp"

namespace PuaraAPI
{

bool DeviceConfiguration::IP1_ready()
{
  if((oscIP1 == "0.0.0.0") || (oscIP1 == ""))
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool DeviceConfiguration::IP2_ready()
{
  if((oscIP2 == "0.0.0.0") || (oscIP2 == ""))
  {
    return false;
  }
  else
  {
    return true;
  }
}

} 