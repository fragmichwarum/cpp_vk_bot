#pragma once

#include <iostream>
#include <fstream>

namespace bot
{
class Logger
{
private:
#if defined(__linux__) || defined(__FreeBSD__)
  const std::string timeLabel_  = "\033[0;35m[ TIME    ]\033[0;0m";
  const std::string logLabel_   = "\033[0;35m[ LOG     ]\033[0;0m";
  const std::string errorLabel_ = "\033[0;31m[ ERROR   ]\033[0;0m";
#else
  const std::string timeLabel_  = "[ TIME    ]";
  const std::string logLabel_   = "[ LOG     ]";
  const std::string errorLabel_ = "[ ERROR   ]";
#endif

  const std::string logPath_;
  const std::string errPath_;

public:
  Logger(const std::string& logPath, const std::string errPath)
    : logPath_(logPath)
    , errPath_(errPath)
  { }

  void log(const std::string& message)
  {
    std::ofstream{logPath_, std::ios::app} << __DATE__ << ' ' << __TIME__ << ' ' << message << std::endl;
  }
  void print(const std::string& message)
  {
    std::cout << logLabel_ << ' ' << __DATE__ << ' ' << __TIME__ << ' ' << message << std::endl;
  }
};
}
