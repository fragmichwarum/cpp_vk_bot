#pragma once

#include <ctime>
#include <fstream>
#include <iosfwd>

class Logger {
private:
  std::string _gen_time();
  std::string _logpath;
  std::string _errpath;

public:
  Logger(std::string const& logpath, std::string const& errpath)
    : _logpath(logpath)
    , _errpath(errpath)
  { }
  void write_log(const std::string& message);
  void write_err(
    long        line,
    const char* file,
    const char* func,
    const char* what
  );
};

