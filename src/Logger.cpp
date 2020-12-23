#include <fstream>
#include <iostream>

#include "Logger.hpp"


void bot::Logger::log(std::string_view message)
{
  std::ofstream{logPath_.data(), std::ios::app} << __DATE__ << ' ' << __TIME__ << ' ' << message << std::endl;
}

void bot::Logger::print(std::string_view message)
{
  std::cout << logLabel_ << ' ' << __DATE__ << ' ' << __TIME__ << ' ' << message << std::endl;
}
