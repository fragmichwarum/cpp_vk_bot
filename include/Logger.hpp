#pragma once

#include <string_view>

namespace bot
{
class Logger
{
private:
#if defined(__linux__) || defined(__FreeBSD__)
  static constexpr std::string_view timeLabel_  = "\033[0;35m[ TIME    ]\033[0;0m";
  static constexpr std::string_view logLabel_   = "\033[0;35m[ LOG     ]\033[0;0m";
  static constexpr std::string_view errorLabel_ = "\033[0;31m[ ERROR   ]\033[0;0m";
#else
  static constexpr std::string_view timeLabel_  = "[ TIME    ]";
  static constexpr std::string_view logLabel_   = "[ LOG     ]";
  static constexpr std::string_view errorLabel_ = "[ ERROR   ]";
#endif

  const std::string_view logPath_;
  const std::string_view errPath_;

public:
  Logger(std::string_view logPath, std::string_view errPath)
    : logPath_(logPath)
    , errPath_(errPath)
  { }

  void log(std::string_view message);
  void print(std::string_view message);
};
}
