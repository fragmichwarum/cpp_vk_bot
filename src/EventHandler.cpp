#include "EventHandler.hpp"

#include "About.hpp"
#include "Cat.hpp"
#include "Complete.hpp"
#include "Document.hpp"
#include "Host.hpp"
#include "Ip.hpp"
#include "Online.hpp"
#include "Picture.hpp"
#include "Role.hpp"
#include "Stat.hpp"
#include "Video.hpp"
#include "Who.hpp"

void bot::EventHandler::initAllCommands()
{
  messageEventhandler.addCommand(new command::About);
  messageEventhandler.addCommand(new command::Cat);
  messageEventhandler.addCommand(new command::Complete);
  messageEventhandler.addCommand(new command::Document);
  messageEventhandler.addCommand(new command::Host);
  messageEventhandler.addCommand(new command::Ip);
  messageEventhandler.addCommand(new command::Online);
  messageEventhandler.addCommand(new command::Picture);
  messageEventhandler.addCommand(new command::Role);
  messageEventhandler.addCommand(new command::Stat);
  messageEventhandler.addCommand(new command::Video);
  messageEventhandler.addCommand(new command::Who);
}

void bot::EventHandler::tryProcessEvent(const simdjson::dom::object& update)
{
  std::string updateType(update["type"].get_c_str());

  if (updateType == "message_new") messageEventhandler.tryProcessMessage(update);
  if (updateType == "wall_post_new") postEventHandler.postMailing(update);
}
