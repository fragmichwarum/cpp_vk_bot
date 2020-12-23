#include <simdjson.h>

#include "PostEventHandler.hpp"
#include "MessageEventHandler.hpp"
#include "EventHandler.hpp"


bot::PostEventHandler* bot::EventHandler::postEventHandler = new bot::PostEventHandler;
bot::MessageEventHandler* bot::EventHandler::messageEventhandler = new bot::MessageEventHandler;

void bot::EventHandler::tryProcessEvent(const simdjson::dom::object& update)
{
  std::string updateType(update["type"].get_c_str());

  if (updateType == "message_new")
  {
    messageEventhandler->tryProcessMessage(
      update["object"]["message"]["text"].get_string(),
      update["object"]["message"]["peer_id"].get_int64()
    );
  }
  if (updateType == "wall_post_new")
  {
    postEventHandler->postMailing(
      update["object"]["from_id"].get_int64(),
      update["object"]["id"].get_int64()
    );
  }
}

bot::EventHandler::~EventHandler()
{
  delete postEventHandler;
  delete messageEventhandler;
}
