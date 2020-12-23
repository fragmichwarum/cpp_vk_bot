#include "Repository/SQLiteRepository.hpp"
#include "VkAPI.hpp"
#include "PostEventHandler.hpp"


bot::VkAPI* bot::PostEventHandler::api = new bot::VkAPI;
bot::Repository* bot::PostEventHandler::repository = new SQLiteRepository("USERS");

void bot::PostEventHandler::updateConversations_()
{
  conversations = repository->getAllConversations();
}

void bot::PostEventHandler::postMailing(long from_id, long id)
{
  updateConversations_();
  std::string attachment =
    std::to_string(from_id) + '_' +
    std::to_string(id);

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
