#pragma once
#include <unordered_map>
#include <vector>

namespace Puara {
struct settingsVariables {
  std::string name;
  std::string type;
  std::string textValue;
  double numberValue;
};

inline std::vector<Puara::settingsVariables> variables;
inline std::unordered_map<std::string, int> variables_fields;

void read_settings_json_internal(std::string& contents, bool merge = false);
void read_config_json_internal(std::string& contents);
void merge_settings_json(std::string& new_contents);
}  // namespace Puara