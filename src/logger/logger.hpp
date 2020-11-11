#pragma once

#include <ctime>
#include <fstream>
#include <iosfwd>

class Logger {
private:
  std::string _logpath;
  std::string _errpath;

#if defined (__linux__)
  const char* eoc    = "\e[0m";
  const char* yellow = "\e[38;5;11m";
  const char* red    = "\e[38;5;1m";
  const char* mint   = "\e[38;5;72m";
#else
  const char* eoc    = "";
  const char* yellow = "";
  const char* red    = "";
  const char* mint   = "";
#endif

public:
  Logger(std::string const& logpath, std::string const& errpath)
    : _logpath(logpath)
    , _errpath(errpath)
  { }
  std::string _gen_time();
  void write_log(const std::string& message);
  void write_err(
    long        line,
    const char* file,
    const char* func,
    const char* what
  );
  void print_log(
    const std::string& message,
    const std::string& from
  );
};

