#include "logger.hpp"

using std::string;
using std::to_string;
using bot::Logger;

string Logger::gen_time() {
  time_t time = std::time(0);
  tm*    now  = std::localtime(&time);
  string month;
  string weekday;
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
         to_string(now->tm_mday) + " " +
         to_string(now->tm_hour) + ":" +
         to_string(now->tm_min)  + ":" +
         to_string(now->tm_sec)  + " " +
         to_string(now->tm_year  + 1900);
}

void Logger::print(uint8_t type, const std::string& message, const std::string& from)
{
  if (type == LOGTYPE::LOG) {
    printf(
      "%s%s%s -> %s%s%s\n %s*%s from id %s%s%s\n\n",
      yellow, gen_time().c_str(),   eoc,
      dim,    message.c_str(),      eoc,
      red,                          eoc,
      mint,   from.c_str(),         eoc
    );
  }
  if (type == LOGTYPE::ERROR) {
    printf(
      "%s%s%s -> %sERROR%s\n %s***%s %s\n\n",
      red, gen_time().c_str(),      eoc,
      red,                          eoc,
      red,                          eoc,
           message.c_str()
    );
  }
}

void Logger::write(uint8_t type, const std::string& message)
{
  if (type == LOGTYPE::LOG) {
    std::ofstream _log(_log_path, std::fstream::app);
    _log << gen_time() << " :\t" << message << '\n';
    _log.close();
  }

  if (type == LOGTYPE::ERROR) {
    std::ofstream _errlog(_error_path, std::fstream::app);
    _errlog << gen_time() << " ERROR: " << message << '\n';
    _errlog.close();
  }
}
