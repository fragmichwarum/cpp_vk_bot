#include <simdjson.h>

#include "PostEventHandler.hpp"
#include "MessageEventHandler.hpp"
#include "EventHandler.hpp"


bot::EventHandler::EventHandler()
  : postEventHandler(std::make_unique<PostEventHandler>())
  , messageEventhandler(std::make_unique<MessageEventHandler>())
{ }

bot::EventHandler::~EventHandler() = default;

void bot::EventHandler::tryProcessEvent(const simdjson::dom::object& update)
{
  std::string updateType(update["type"].get_c_str());

  if (updateType == "message_new")
  {
    messageEventhandler->process(
      update["object"]["message"]["text"].get_string(),
      update["object"]["message"]["peer_id"].get_int64(),
      update["object"]["message"]["from_id"].get_int64()
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
