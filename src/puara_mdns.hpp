#pragma once
#include <string_view>
namespace PuaraImpl {
void start_mdns_service(std::string_view device_name, std::string_view instance_name);
}