#pragma once
#include <string_view>
namespace PuaraAPI
{
struct MDNSService
{
  void start(std::string_view device_name, std::string_view instance_name);
};
} 