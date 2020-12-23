#pragma once

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
private:
  static class PostEventHandler* postEventHandler;
  static class MessageEventHandler* messageEventhandler;

public:
  /*!
   * @brief Check update type and try process it.
   * @param JSON update.
   */
  void tryProcessEvent(const simdjson::dom::object& update);
 ~EventHandler();
};
}
