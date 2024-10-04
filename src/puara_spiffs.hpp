#pragma once
#include <unordered_map>
#include <vector>

namespace PuaraImpl {
struct settingsVariables {
  std::string name;
  std::string type;
  std::string textValue;
  double numberValue;
};

// Public api
void config_spiffs();
void mount_spiffs();
void unmount_spiffs();

void read_config_json();
void write_config_json();
void read_settings_json();
void write_settings_json();

double getVarNumber(std::string varName);
std::string getVarText(std::string varName);

// Private API used by puara_serial
inline std::vector<PuaraImpl::settingsVariables> variables;
inline std::unordered_map<std::string, int> variables_fields;

void read_settings_json_internal(std::string& contents, bool merge = false);
void read_config_json_internal(std::string& contents);
void merge_settings_json(std::string& new_contents);

}  // namespace PuaraImpl