#include <fstream>
#include <iostream>
#include <ctime>

#include "Logger.hpp"


std::string bot::Logger::time() noexcept
{
  time_t time = std::time(0);
  tm*    now  = std::localtime(&time);
  std::string month;
  std::string weekday;
  switch (now->tm_mon + 1)
  {
    case 1:
      month = "Jan";
      break;
    case 2:
      month = "Feb";
      break;
    case 3:
      month = "Mar";
      break;
    case 4:
      month = "Apr";
      break;
    case 5:
      month = "May";
      break;
    case 6:
      month = "Jun";
      break;
    case 7:
      month = "Jul";
      break;
    case 8:
      month = "Aug";
      break;
    case 9:
      month = "Sep";
      break;
    case 10:
      month = "Oct";
      break;
    case 11:
      month = "Nov";
      break;
    case 12:
      month = "Dec";
      break;
  }

  switch (now->tm_wday)
  {
    case 1:
      weekday = "Mon";
      break;
    case 2:
      weekday = "Tue";
      break;
    case 3:
      weekday = "Wed";
      break;
    case 4:
      weekday = "Thu";
      break;
    case 5:
      weekday = "Fri";
      break;
    case 6:
      weekday = "Sat";
      break;
    case 7:
      weekday = "Sun";
      break;
  }
  return weekday + " " + month   + " " +
         ((now->tm_mday < 10) ? "0" : "") + std::to_string(now->tm_mday) + " " +
         ((now->tm_hour < 10) ? "0" : "") + std::to_string(now->tm_hour) + ":" +
         ((now->tm_min  < 10) ? "0" : "") + std::to_string(now->tm_min)  + ":" +
         ((now->tm_sec  < 10) ? "0" : "") + std::to_string(now->tm_sec)  + " " +
         std::to_string(now->tm_year  + 1900);
}

void bot::Logger::log(std::string_view message) noexcept
{
  std::ofstream{logPath_.data(), std::ios::app} << time() << ' ' << message << std::endl;
}

void bot::Logger::print(std::string_view message) noexcept
{
  std::cout << logLabel_ << time() << ' ' << message << std::endl;
}
