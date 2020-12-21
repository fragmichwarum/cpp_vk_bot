#include "Repository/SQLiteRepository.hpp"
#include "VkAPI.hpp"
#include "PostEventHandler.hpp"

//extern template class std::vector<long>;

bot::VkAPI* bot::PostEventHandler::api = new bot::VkAPI;
bot::Repository* bot::PostEventHandler::repository = new SQLiteRepository("USERS");

void bot::PostEventHandler::updateConversations_()
{
  conversations = repository->getAllConversations();
}

void bot::PostEventHandler::postMailing(const simdjson::dom::object& update)
{
  updateConversations_();
  std::string attachment =
    std::to_string(update["object"]["from_id"].get_int64()) + '_' +
    std::to_string(update["object"]["id"].get_int64());

  for(const long& conversation : conversations)
  {
    api->sendMessage("Таки новый пост в группе🌚", conversation, {{"attachment", "wall" + attachment}});
  }
}

bot::PostEventHandler::~PostEventHandler()
{
  delete api;
  delete repository;
}
