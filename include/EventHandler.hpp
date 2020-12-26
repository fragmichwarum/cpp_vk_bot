#pragma once

#include <memory>

namespace simdjson::dom
{
class object;
}

namespace bot
{
/*!
 * @brief Common VK events handler.
 */
class EventHandler
{
public:
  explicit
  EventHandler();
  /*!
   * @brief Destructor marked as <em>default</em>.
   *
   * It must be defined in place, when <em>std::unique_ptr</em> pointed-to type is complete.
   */
 ~EventHandler();
  /*!
   * @brief Check update type and try process it.
   * @param JSON update.
   */
  void tryProcessEvent(const simdjson::dom::object& update);

private:
  std::unique_ptr<class PostEventHandler> postEventHandler;
  std::unique_ptr<class MessageEventHandler> messageEventhandler;
};
}
