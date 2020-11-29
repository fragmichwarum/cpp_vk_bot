#pragma once

#include "Logger.hpp"

namespace bot
{
class EventLogger final : protected Logger
{
public:
  EventLogger(const std::string& logpath)
    : Logger(logpath)
  { }
  void log(const std::string& message) const override
  {
    std::ofstream{_logpath, std::ios::app} << getTime() << ": " << message << std::endl;
  }
  void print(const std::string& message) const override
  {
    std::cout << getTime() << ": " << message << std::endl;
  }
};
} //namespace bot
