#pragma once

#include <string_view>


namespace bot::third_party
{
class logger
{
public:
  explicit
  logger(std::string_view log_path)
  noexcept
    : _log_path(log_path)
  { }

  void log(std::string_view);
  void print(std::string_view);

private:
#if defined(__linux__) || defined(__FreeBSD__)
  inline static constexpr std::string_view _time_label = "\033[0;35m[ TIME    ]\033[0;0m";
  inline static constexpr std::string_view _log_label  = "\033[0;35m[ LOG     ]\033[0;0m";
  inline static constexpr std::string_view _err_label  = "\033[0;31m[ ERROR   ]\033[0;0m";
#else
  inline static constexpr std::string_view _time_label = "[ TIME    ]";
  inline static constexpr std::string_view _log_label  = "[ LOG     ]";
  inline static constexpr std::string_view _err_label  = "[ ERROR   ]";
#endif

  const std::string_view _log_path;

  std::string time() noexcept;
};
}
