#include "PostEventHandler.hpp"
#include "MessageEventHandler.hpp"
#include "EventHandler.hpp"

bot::PostEventHandler* bot::EventHandler::postEventHandler_ = new bot::PostEventHandler;
bot::MessageEventHandler* bot::EventHandler::messageEventhandler_ = new bot::MessageEventHandler;

void bot::EventHandler::tryProcessEvent(const simdjson::dom::object& update)
{
  std::string updateType(update["type"].get_c_str());

  if (updateType == "message_new") messageEventhandler_->tryProcessMessage(update);
  if (updateType == "wall_post_new") postEventHandler_->postMailing(update);
}

bot::EventHandler::~EventHandler()
{
  delete postEventHandler_;
  delete messageEventhandler_;
}
