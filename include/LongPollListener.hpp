#pragma once

#include "VKError.hpp"
#include "EventHandler.hpp"

namespace bot
{
class LongPollListener
{
private:
  std::string server;
  std::string key;
  std::string ts;

  simdjson::dom::parser parser;
  EventHandler eventHandler;
  VkAPI* api = VkAPI::getInstance();

  void updateLongPollData();
  void processEvents(const simdjson::dom::array& updates);

public:
  void loop();
};
} //namespace bot
