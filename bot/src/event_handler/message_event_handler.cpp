#include <iostream>

#include "../cpp_vk_lib/logger/logger.hpp"
#include "../bot/include/event_handler/message_event_handler.hpp"

#include "../bot/include/commands/about.hpp"
#include "../bot/include/commands/cat.hpp"
#include "../bot/include/commands/chat_name.hpp"
#include "../bot/include/commands/complete.hpp"
#include "../bot/include/commands/currency.hpp"
#include "../bot/include/commands/docs.hpp"
#include "../bot/include/commands/exit.hpp"
#include "../bot/include/commands/host.hpp"
#include "../bot/include/commands/ip.hpp"
#include "../bot/include/commands/kick.hpp"
#include "../bot/include/commands/mandelbrot_set.hpp"
#include "../bot/include/commands/online.hpp"
#include "../bot/include/commands/photo_loader.hpp"
#include "../bot/include/commands/pics.hpp"
#include "../bot/include/commands/role.hpp"
#include "../bot/include/commands/stat.hpp"
#include "../bot/include/commands/videos.hpp"
#include "../bot/include/commands/who.hpp"


bot::message_event_handler::~message_event_handler() = default;

bot::message_event_handler::message_event_handler()
{
  command_collection.emplace("+оботе",        std::make_unique<command::about>());
  command_collection.emplace("+котик",        std::make_unique<command::cat>());
  command_collection.emplace("+чат",          std::make_unique<command::chat_name>());
  command_collection.emplace("+дополни",      std::make_unique<command::complete>());
  command_collection.emplace("+курс",         std::make_unique<command::currency>());
  command_collection.emplace("+доки",         std::make_unique<command::docs>());
  command_collection.emplace("+офф",          std::make_unique<command::exit>());
  command_collection.emplace("+хост",         std::make_unique<command::host>());
  command_collection.emplace("+айпи",         std::make_unique<command::ip>());
  command_collection.emplace("+кик",          std::make_unique<command::kick>());
  command_collection.emplace("+мандельброт",  std::make_unique<command::mandelbrot_set>());
  command_collection.emplace("+онлайн",       std::make_unique<command::online>());
  command_collection.emplace("+загрузи",      std::make_unique<command::photo_loader>());
  command_collection.emplace("+пикча",        std::make_unique<command::pics>());
  command_collection.emplace("+стат",         std::make_unique<command::stat>());
  command_collection.emplace("+видео",        std::make_unique<command::videos>());
  command_collection.emplace("+кто",          std::make_unique<command::who>());
}

void bot::message_event_handler::process(const vk::event::message_new& event)
{
  std::string trigger = event.text().substr(0, event.text().find(' '));

  if (command_collection.find(trigger) != command_collection.end())
  {
    logger("/home/gentoo/botlog.txt", logflag::info | logflag::yellow | logflag::spaces) << event.text();
    command_collection[trigger]->execute(event);
  }
}
