#include "PostEventHandler.hpp"

bot::PostEventHandler::PostEventHandler()
  : repository(bot::Repository::getInstance())
{ }

void bot::PostEventHandler::updateConversations()
{
  conversations = repository->getAllConversations();
}

void bot::PostEventHandler::postMailing(const simdjson::dom::object& update)
{
  updateConversations();
  std::string attachment =
    std::to_string(update["object"]["from_id"].get_int64()) + '_' +
    std::to_string(update["object"]["id"].get_int64());

  for(const long& conversation : conversations)
  {
    api->sendMessage("Таки новый пост в группе🌚", conversation, {{"attachment", "wall" + attachment}});
  }
}
