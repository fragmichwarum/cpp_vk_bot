#pragma once

#include <fstream>
#include <iostream>

#include "Logger.hpp"

namespace bot
{
class ErrorLogger final : protected Logger
{
public:
  ErrorLogger(const std::string& logpath)
    : Logger(logpath)
  { }
  void log(const std::string& message) const override
  {
    std::ofstream{_logpath, std::ios::app} << getTime() << ": " << "ERROR: " << message << std::endl;
  }
  void print(const std::string& message) const override
  {
    std::cout << getTime() << " ERROR: " << message << std::endl;
  }
};
} //namespace bot
