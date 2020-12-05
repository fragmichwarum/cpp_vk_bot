#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace bot
{
class Logger
{
private:
  const std::string timeLabel_  = "\033[0;35m[ TIME    ]\033[0;0m";
  const std::string debugLabel_ = "\033[0;35m[ DEBUG   ]\033[0;0m";
  const std::string errorLabel_ = "\033[0;31m[ ERROR   ]\033[0;0m";

  const std::string logPath_;
  const std::string errPath_;

public:
  Logger(const std::string& logPath, const std::string errPath)
    : logPath_(logPath)
    , errPath_(errPath)
  { }

  void log(const std::string& message)
  {
    std::ofstream{logPath_, std::ios::app} << __DATE__ << ' ' << __TIME__ <<  message;
  }
  void print(const std::string& message)
  {
    std::cout << __DATE__ << ' ' << __TIME__ <<  message << std::endl;
  }
};
}
