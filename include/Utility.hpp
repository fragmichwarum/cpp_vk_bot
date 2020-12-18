#pragma once

#include <string>
#include <vector>
#include <map>

namespace bot
{
namespace util
{
std::string toJson(const std::map<std::string, std::string>& body);
std::string longToHexStr(unsigned long digit) noexcept;
std::vector<std::string> split(const std::string& text);
std::string getArgs(const std::string& message);
std::string emptyArgs() noexcept;
long extractId(const std::string& user);
} //namespace util
} //namespace bot
