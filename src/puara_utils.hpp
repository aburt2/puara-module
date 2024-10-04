#pragma once
#include <string>

namespace PuaraImpl {

void find_and_replace(std::string old_text, std::string new_text, std::string& str);
void find_and_replace(std::string old_text, double new_number, std::string& str);
void find_and_replace(std::string old_text, unsigned int new_number, std::string& str);
void checkmark(std::string old_text, bool value, std::string& str);
std::string urlDecode(std::string text);
std::string convertToString(char* a);

}  // namespace PuaraImpl