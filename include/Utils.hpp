#pragma once

#include <vector>
#include <numeric>
#include <sstream>
#include <iterator>

namespace bot
{
namespace util
{
std::string longToHexStr(unsigned long digit) noexcept;
std::vector<std::string> split(const std::string& text);
std::string getArgs(const std::string& message);
std::string emptyArgs() noexcept;
}
}
