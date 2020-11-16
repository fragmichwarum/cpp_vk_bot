#pragma once

#include <ctime>
#include <fstream>
#include <iostream>

namespace bot
{
/*!
 * @brief Enumeration of logging types.
 */
typedef enum : uint8_t {
  LOG,
  ERROR
} LOGTYPE;
/*!
 * @brief Class providing logging capability.
 */
class Vk_logger
{
protected:
#if defined (__linux__) || defined (__FreeBSD__)
  const char* eoc    = "\e[0m";
  const char* yellow = "\e[38;5;11m";
  const char* red    = "\e[38;5;1m";
  const char* mint   = "\e[38;5;72m";
  const char* dim    = "\e[2m";
#else
  const char* eoc    = "";
  const char* yellow = "";
  const char* red    = "";
  const char* mint   = "";
#endif
  /*!
   * @brief Path to log file.
   */
  std::string _log_path;
  /*!
   * @brief Path to error log file.
   */
  std::string _err_path;

  /*!
   * @brief Generate date & time.
   * @return Date in format EEE MMM DD HH MM SS YYYY at this moment.
   */
  std::string gen_time();
  /*!
   * @brief Colorize text(Bash).
   * @param color to be applied.
   * @param text to be colorized.
   * @return Colorized string.
   */
  std::string colorize(const std::string& color, const std::string& text);
  /*!
   * @brief Generate message to be logged
   * @param message
   * @param from_id
   * @return [time] -> colorized(message)\n * colorized(from_id).
   */
  std::string log_format(const std::string& message, const std::string& from_id);
  /*!
   * @brief Generate error message to be logged
   * @param message
   * @return [time] -> ERROR:\n * colorized(message).
   */
  std::string err_format(const std::string& message);

public:
  Vk_logger(const std::string& log_path, const std::string err_path)
    : _log_path(log_path)
    , _err_path(err_path)
  { }
  /*!
   * @brief Print logging message to screen.
   * @param type     - any member of available @ref bot::LOGTYPE "logging types".
   * @param message  - message to be printed.
   * @param from_id  - (optional) user_id to be printed.
   */
  void print(uint8_t type, const std::string& message, const std::string& from_id = 0);
  /*!
   * @brief Write logging message to
   * @ref bot::Vk_logger::_log_path "log file" or
   * @ref bot::Vk_logger::_err_path "error log file" file depending on <b><em>type</em></b> param.
   * @param type     - any member of available @ref bot::LOGTYPE "logging types".
   * @param message  - message to be printed.
   * @param from_id  - (optional) user_id to be printed.
   */
  void log(uint8_t type, const std::string& message);
};
} //namespace bot
