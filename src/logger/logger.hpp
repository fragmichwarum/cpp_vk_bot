#pragma once

#include <ctime>
#include <fstream>
#include <iosfwd>

class Logger {
private:
  std::string   _gen_time();
  std::ofstream _log;
  std::ofstream _err;

public:
  Logger(const std::string& logpath, const std::string& errpath)
    : _log (logpath, std::ios::app)
    , _err (errpath, std::ios::app)
  { }
  void write_log(const std::string& message);
  void write_err(
    long        line,
    const char* file,
    const char* func,
    const char* what
  );
};

