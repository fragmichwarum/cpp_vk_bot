#include "logger.hpp"

using std::string;
using std::to_string;
using bot::Vk_logger;
using bot::LOGTYPE;

string Vk_logger::gen_time() {
  time_t time = std::time(0);
  tm*    now  = std::localtime(&time);
  string month;
  string weekday;
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
         to_string(now->tm_mday) + " " +
         to_string(now->tm_hour) + ":" +
         to_string(now->tm_min)  + ":" +
         to_string(now->tm_sec)  + " " +
         to_string(now->tm_year  + 1900);
}

void Vk_logger::print(uint8_t type, const std::string &message, const std::string& from_id)
{
  switch (type)
  {
    case LOGTYPE::LOG:
      std::cout << log_format(message, from_id);
      break;

    case LOGTYPE::ERROR:
      std::cout << err_format(message);
      break;
  }
}

void Vk_logger::log(uint8_t type, const std::string& message)
{
  switch (type)
  {
    case LOGTYPE::LOG:
      std::ofstream{_log_path, std::fstream::app}
        << gen_time() << " :\t" << message << '\n';
      break;

    case LOGTYPE::ERROR:
      std::ofstream{_err_path, std::fstream::app}
        << gen_time() << " ERROR: " << message << '\n';
      break;
  }
}

string Vk_logger::colorize(const std::string& color, const std::string& text)
{
  return color + text + eoc;
}

string Vk_logger::log_format(const std::string& message, const std::string& from_id)
{
  return colorize(yellow, '[' +gen_time() + ']') + "\t -> " +
         colorize(dim, message)           + "\n " +
         colorize(red, "*")               + "  from id " +
         colorize(mint, from_id)          + "\n\n";
}

string Vk_logger::err_format(const std::string& message)
{
  return colorize(yellow, '[' +gen_time() + ']') + "\t -> " +
         colorize(red, "ERROR")    + "\n"   +
         colorize(red, message)    + "\n\n";
}
