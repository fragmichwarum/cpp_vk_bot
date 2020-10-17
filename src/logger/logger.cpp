#include "logger.hpp"

std::string Logger::_gen_time() {
  std::time_t time = std::time(0);
  std::tm* now = std::localtime(&time);
  std::string month;
  std::string weekday;
  switch (now->tm_mon + 1) {
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

  switch (now->tm_wday) {
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
  return weekday + " " + month + " " +
         std::to_string(now->tm_mday) + " " +
         std::to_string(now->tm_hour) + ":" +
         std::to_string(now->tm_min)  + ":" +
         std::to_string(now->tm_sec)  + " " +
         std::to_string(now->tm_year  + 1900);
}


void Logger::write_log(const std::string& message) {
  std::ofstream out(_path, std::ios::app);
  out << _gen_time() << " -> " << message << '\n';
}
