#pragma once

#include <ctime>
#include <fstream>
#include <iostream>

namespace bot
{
typedef enum : uint8_t {
  LOG,
  ERROR
} LOGTYPE;

class Vk_logger
{
protected:
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
  std::string _log_path;
  std::string _err_path;

  std::string gen_time();
  std::string colorize(const std::string& color, const std::string& text);
  std::string log_format(const std::string& message, const std::string& from_id);
  std::string err_format(const std::string& message);

public:
  Vk_logger(const std::string& log_path, const std::string err_path)
    : _log_path(log_path)
    , _err_path(err_path)
  { }
  void print(uint8_t type, const std::string& message, const std::string& from_id = 0);
  void log(uint8_t type, const std::string& message);
};
} //namespace bot
