#pragma once

#include <vector>
#include <sstream>
#include <iterator>

namespace bot
{
namespace util
{
std::vector<std::string> split(const std::string& text);
std::string get_args(const std::string& message);
std::string empty_args();
} // util
} // bot
