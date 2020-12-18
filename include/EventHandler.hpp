#pragma once

#include <simdjson.h>

namespace bot
{
class EventHandler
{
private:
  static class PostEventHandler* postEventHandler_;
  static class MessageEventHandler* messageEventhandler_;

public:
  void tryProcessEvent(const simdjson::dom::object& update);
 ~EventHandler();
};
}
