#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <ctime>
#include <fstream>
#include <ios>

class Logger {
private:
  std::string _gen_time();
  std::string _path;
  std::ofstream _out;

public:
  Logger(const std::string& path)
    : _path(path)
    , _out (_path, std::ios::app)
  { }
  void write_log(const std::string& message);
};

#endif //LOGGER_HPP
