#pragma once

#include <simdjson.h>

namespace bot
{
class PostEventHandler
{
private:
  std::vector<long> conversations_;
  static class Repository* repository_;
  static class VkAPI* api_;

  void updateConversations_();

public:
  void postMailing(const simdjson::dom::object& update);
 ~PostEventHandler();
};
}
