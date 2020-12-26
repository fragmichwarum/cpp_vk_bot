#include "Repository/SQLiteRepository.hpp"
#include "VkAPI.hpp"
#include "PostEventHandler.hpp"


bot::PostEventHandler::PostEventHandler()
  : repository(std::make_unique<SQLiteRepository>("USERS"))
  , api(std::make_unique<VkAPI>())
{ }

bot::PostEventHandler::~PostEventHandler() = default;

void bot::PostEventHandler::updateConversations_()
{
  conversations = repository->getAllConversations();
}

void bot::PostEventHandler::postMailing(long from_id, long id)
{
  updateConversations_();
  std::string attachment = std::to_string(from_id) + '_' + std::to_string(id);

  for(long conversation : conversations)
  {
    api->sendMessage("Таки новый пост в группе🌚", conversation, {{"attachment", "wall" + attachment}});
  }
}
