#pragma once

#include "PostEventHandler.hpp"
#include "MessageEventHandler.hpp"

namespace bot
{
class EventHandler
{
private:
  PostEventHandler postEventHandler;
  MessageEventHandler messageEventhandler;

public:
  void initAllCommands();
  void tryProcessEvent(const simdjson::dom::object& update);
};
}
