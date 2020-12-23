#include "Network.hpp"
#include "Utility.hpp"
#include "Logger.hpp"
#include "VkAPI.hpp"
#include "Repository/SQLiteRepository.hpp"
#include "MessageEventHandler.hpp"

#include "About.hpp"
#include "Cat.hpp"
#include "ChatName.hpp"
#include "Complete.hpp"
#include "Currency.hpp"
#include "Document.hpp"
#include "Host.hpp"
#include "Ip.hpp"
#include "Online.hpp"
#include "Picture.hpp"
#include "Role.hpp"
#include "Stat.hpp"
#include "Video.hpp"
#include "Who.hpp"

#include <iostream>


bot::Network* net                              = new bot::Network;
bot::Repository* repository                    = new bot::SQLiteRepository("USRES");
bot::VkAPI*   bot::MessageEventHandler::api    = new bot::VkAPI;
bot::Logger*  bot::MessageEventHandler::logger = new bot::Logger(api->getLogPath(), api->getErrPath());

bot::MessageEventHandler::MessageEventHandler()
{
  commandCollection.emplace("+оботе",    std::make_unique<command::About>());
  commandCollection.emplace("+котик",    std::make_unique<command::Cat>());
  commandCollection.emplace("+чатнейм",  std::make_unique<command::ChatNameSetter>());
  commandCollection.emplace("+дополни",  std::make_unique<command::Complete>());
  commandCollection.emplace("+курс",     std::make_unique<command::Currency>());
  commandCollection.emplace("+доки",     std::make_unique<command::Document>());
  commandCollection.emplace("+хост",     std::make_unique<command::Host>());
  commandCollection.emplace("+айпи",     std::make_unique<command::Ip>());
  commandCollection.emplace("+онлайн",   std::make_unique<command::Online>());
  commandCollection.emplace("+пикча",    std::make_unique<command::Picture>());
  commandCollection.emplace("+роль",     std::make_unique<command::Role>());
  commandCollection.emplace("+стат",     std::make_unique<command::Stat>());
  commandCollection.emplace("+видео",    std::make_unique<command::Video>());
  commandCollection.emplace("+кто",      std::make_unique<command::Who>());
}

void bot::MessageEventHandler::processLogging(std::string_view message)
{
  logger->log(message.data());
  logger->print(message.data());
}

void bot::MessageEventHandler::processHelp(long peer_id)
{
  static std::string help;
  if (help.empty())
  {
    for (const std::pair<const std::string_view, std::unique_ptr<ICommand>>& command : commandCollection)
    {
      help.append(command.first);
      help.append(" - ");
      help.append(command.second->description());
      help.append("\n");
    }
  }
  api->sendMessage(help, peer_id);
}

void bot::MessageEventHandler::tryProcessMessage(std::string_view message, long peer_id)
{
  if (message.empty()     ) return;
  if (message.at(0) != '+') return;
  if (message == "+помощь") processHelp(peer_id);
  processLogging(message);

  std::string_view trigger = message.substr(0, message.find_first_of(" "));
  if (commandCollection.find(trigger) != commandCollection.end())
  {
    std::string commandAnswer = commandCollection[trigger]->execute({util::getArgs(message).data(), peer_id}, {repository, api, net});
    if (not commandAnswer.empty()) api->sendMessage(commandAnswer, peer_id);
  }
}

bot::MessageEventHandler::~MessageEventHandler()
{
  delete api;
  delete logger;
  delete repository;
  delete net;
}
