#pragma once

#include <vector>
#include <numeric>
#include <sstream>
#include <iterator>

namespace bot
{
namespace util
{
std::string long_to_hex_str(unsigned long digit) noexcept(true);
std::vector<std::string> split(const std::string& text);
std::string get_args(const std::string& message);
std::string empty_args();
} // util
} // bot
