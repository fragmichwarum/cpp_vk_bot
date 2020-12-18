#pragma once

#include <simdjson.h>

namespace bot
{
class LongPollListener
{
private:
  std::string server_;
  std::string key_;
  std::string ts_;

  static class EventHandler* eventHandler_;
  static class VkAPI* api_;

  void updateLongPollData_();
  void processEvents_(const simdjson::dom::array& updates);

public:
  void loop();
 ~LongPollListener();
};
} //namespace bot
