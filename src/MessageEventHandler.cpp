#include "Network.hpp"
#include "Utility.hpp"
#include "Logger.hpp"
#include "VkAPI.hpp"
#include "MessageEventHandler.hpp"
#include "Repository/SQLiteRepository.hpp"

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

bot::Network* net                              = new bot::Network;
bot::Repository* repository                    = new bot::SQLiteRepository("USRES");
bot::VkAPI*   bot::MessageEventHandler::api    = new bot::VkAPI;
bot::Logger*  bot::MessageEventHandler::logger = new bot::Logger(api->getLogPath(), api->getErrPath());

bot::MessageEventHandler::MessageEventHandler()
{
  commandCollection.emplace("+оботе",    std::make_shared<command::About>());
  commandCollection.emplace("+котик",    std::make_shared<command::Cat>());
  commandCollection.emplace("+чатнейм",  std::make_shared<command::ChatNameSetter>());
  commandCollection.emplace("+дополни",  std::make_shared<command::Complete>());
  commandCollection.emplace("+курс",     std::make_shared<command::Currency>());
  commandCollection.emplace("+доки",     std::make_shared<command::Document>());
  commandCollection.emplace("+хост",     std::make_shared<command::Host>());
  commandCollection.emplace("+айпи",     std::make_shared<command::Ip>());
  commandCollection.emplace("+онлайн",   std::make_shared<command::Online>());
  commandCollection.emplace("+пикча",    std::make_shared<command::Picture>());
  commandCollection.emplace("+роль",     std::make_shared<command::Role>());
  commandCollection.emplace("+стат",     std::make_shared<command::Stat>());
  commandCollection.emplace("+видео",    std::make_shared<command::Video>());
  commandCollection.emplace("+кто",      std::make_shared<command::Who>());
}

void bot::MessageEventHandler::processLogging_(std::string_view message)
{
  logger->log(message.data());
  logger->print(message.data());
}

void bot::MessageEventHandler::processHelp_(const long& peer_id)
{
  std::string help;
  for (const std::pair<const std::string, std::shared_ptr<ICommand>>& command : commandCollection)
  {
    help.append(command.first);
    help.append(" - ");
    help.append(command.second->description());
    help.append("\n");
  }
  api->sendMessage(help, peer_id);
}

void bot::MessageEventHandler::tryProcessMessage(const simdjson::dom::object& update)
{
  std::string message(update["object"]["message"]["text"].get_c_str());
  long peer_id(update["object"]["message"]["peer_id"].get_int64());

  if (message.empty()) return;
  if (message.at(0) != '+') return;
  if (message == "+помощь") processHelp_(peer_id);

//  processLogging_(message);

  std::string trigger = message.substr(0, message.find_first_of(" "));
  if (commandCollection.find(trigger) != commandCollection.end())
  {
    std::string commandAnswer = commandCollection[trigger]->execute({util::getArgs(message), peer_id}, {repository, api, net});
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
