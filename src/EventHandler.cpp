#include "PostEventHandler.hpp"
#include "MessageEventHandler.hpp"
#include "EventHandler.hpp"

extern template class std::basic_string<char>;

bot::PostEventHandler* bot::EventHandler::postEventHandler = new bot::PostEventHandler;
bot::MessageEventHandler* bot::EventHandler::messageEventhandler = new bot::MessageEventHandler;

void bot::EventHandler::tryProcessEvent(const simdjson::dom::object& update)
{
  std::string updateType(update["type"].get_c_str());

  if (updateType == "message_new") messageEventhandler->tryProcessMessage(update);
  if (updateType == "wall_post_new") postEventHandler->postMailing(update);
}

bot::EventHandler::~EventHandler()
{
  delete postEventHandler;
  delete messageEventhandler;
}
