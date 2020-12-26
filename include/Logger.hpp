#pragma once

#include <string_view>

namespace bot
{
class Logger
{
public:
  explicit
  Logger(std::string_view logPath, std::string_view errPath)
  noexcept
    : logPath_(logPath)
    , errPath_(errPath)
  { }

  void log(std::string_view message) noexcept;
  void print(std::string_view message) noexcept;

private:
#if defined(__linux__) || defined(__FreeBSD__)
  inline static constexpr std::string_view timeLabel_  = "\033[0;35m[ TIME    ]\033[0;0m";
  inline static constexpr std::string_view logLabel_   = "\033[0;35m[ LOG     ]\033[0;0m";
  inline static constexpr std::string_view errorLabel_ = "\033[0;31m[ ERROR   ]\033[0;0m";
#else
  inline static constexpr std::string_view timeLabel_  = "[ TIME    ]";
  inline static constexpr std::string_view logLabel_   = "[ LOG     ]";
  inline static constexpr std::string_view errorLabel_ = "[ ERROR   ]";
#endif

  const std::string_view logPath_;
  const std::string_view errPath_;

  std::string time() noexcept;
};
}
