#pragma once

#include <string>

namespace simdjson::dom
{
class array;
}

namespace bot
{
/*!
 * @brief The LongPollListener class
 */
class LongPollListener
{
private:
  std::string server;
  std::string key;
  std::string ts;

  static class EventHandler* eventHandler;

  /*!
   * @brief get Long Poll server and initialize @ref server, @ref key and @ref ts
   */
  void updateLongPollData_();
  /*!
   * @brief Iterate through updates.
   * @param JSON update.
   */
  void processEvents_(const simdjson::dom::array& updates);

public:
  /*!
   * @brief Start point of application.
   */
  void loop();
 ~LongPollListener();
};
} //namespace bot
