#pragma once

#include <string>
#include <map>


namespace bot::third_party::string_utils
{
std::string cut_first(std::string_view);
std::string to_hex(unsigned long);
std::string to_json(const std::map<std::string, std::string>& body);
}
