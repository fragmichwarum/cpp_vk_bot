#pragma once

#include <string>
#include <memory>

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
public:
  explicit
  LongPollListener();
  /*!
   * @brief Destructor marked as <em>default</em>.
   *
   * It must be defined in place, when <em>std::unique_ptr</em> pointed-to type is complete.
   */
 ~LongPollListener();
  /*!
   * @brief Start point of application.
   */
  void loop();

private:
  std::string server;
  std::string key;
  std::string ts;

  std::unique_ptr<class EventHandler> eventHandler;
  std::unique_ptr<class VkAPI> api;

  /*!
   * @brief get Long Poll server and initialize @ref server, @ref key and @ref ts
   */
  void updateLongPollData_();
  /*!
   * @brief Iterate through updates.
   * @param JSON update.
   */
  void processEvents_(const simdjson::dom::array& updates);
};
} //namespace bot
