#pragma once

#include "Repository.hpp"
#include "VKAPI.hpp"

namespace bot
{
class PostEventHandler
{
private:
  using conversation = long;
  std::vector<conversation> conversations;
  Repository* repository;
  VkAPI* api = VkAPI::getInstance();

  void updateConversations();

public:
  PostEventHandler();
  void postMailing(const simdjson::dom::object& update);
};
}
