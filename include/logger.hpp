#pragma once

#include <ctime>
#include <fstream>
#include <iosfwd>

namespace bot
{
typedef enum : uint8_t {
  LOG,
  ERROR
} LOGTYPE;

class Logger {
public:
  Logger(const std::string& log_path, const std::string& error_path)
    : _log_path  (log_path)
    , _error_path(error_path)
  { }

  std::string gen_time();

  void print(uint8_t type, const std::string& message, const std::string& from = 0);

  void write(uint8_t type, const std::string& message);

private:
  std::string _log_path;

  std::string _error_path;

#if defined (__linux__) || defined (__FreeBSD__)
  const char* eoc    = "\e[0m";
  const char* yellow = "\e[38;5;11m";
  const char* red    = "\e[38;5;1m";
  const char* mint   = "\e[38;5;72m";
  const char* dim    = "\e[2m";
#else
  const char* eoc    = "";
  const char* yellow = "";
  const char* red    = "";
  const char* mint   = "";
#endif
};
} //namespace bot
