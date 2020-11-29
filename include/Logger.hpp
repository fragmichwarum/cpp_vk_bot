#pragma once

#include <fstream>
#include <iostream>
#include <ctime>

namespace bot
{
class Logger
{
protected:
  std::string _logpath;

  Logger(const std::string& logpath)
    : _logpath(logpath)
  { }
  std::string getTime() const
  {
    std::time_t ct = std::time(0);
    std::string time = ctime(&ct);
    time.pop_back();
    return time;
  }
  virtual void log(const std::string& message) const = 0;
  virtual void print(const std::string& message) const = 0;
};
} //namespace bot
