#include "logger.hpp"

using std::string;
using std::ofstream;
using std::time_t;
using std::tm;
using std::to_string;

string Logger::_gen_time() {
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

void Logger::write_log(const string& message) {
  _log << _gen_time() << ":\t" << message << '\n';
}

void Logger::write_err(
    long        line,
    const char* file,
    const char* func,
    const char* what
) {
  _err << _gen_time() << "\t" << file << ':' << to_string(line)
       << " in " << func << " -\t" << what << '\n';
}
