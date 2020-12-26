#include "Utility.hpp"
#include "Logger.hpp"
#include "VkAPI.hpp"
#include "JsonUtils.hpp"
#include "Repository/SQLiteRepository.hpp"
#include "MessageEventHandler.hpp"

#include "About.hpp"
#include "Cat.hpp"
#include "ChatName.hpp"
#include "Complete.hpp"
#include "Currency.hpp"
#include "Document.hpp"
#include "Exit.hpp"
#include "Host.hpp"
#include "Ip.hpp"
#include "Kick.hpp"
#include "Online.hpp"
#include "Picture.hpp"
#include "Ping.hpp"
#include "Role.hpp"
#include "Stat.hpp"
#include "Video.hpp"
#include "Who.hpp"


std::unique_ptr<bot::Repository> repository = std::make_unique<bot::SQLiteRepository>("USERS");
std::unique_ptr<bot::JsonUtils> jsonUtils = std::make_unique<bot::JsonUtils>();

bot::MessageEventHandler::MessageEventHandler()
  : api   (std::make_unique<VkAPI>())
  , logger(std::make_unique<Logger>(api->getLogPath(), api->getErrPath()))
{
  commandCollection.emplace("+оботе",    std::make_unique<command::About>());
  commandCollection.emplace("+котик",    std::make_unique<command::Cat>());
  commandCollection.emplace("+чатнейм",  std::make_unique<command::ChatNameSetter>());
  commandCollection.emplace("+дополни",  std::make_unique<command::Complete>());
  commandCollection.emplace("+курс",     std::make_unique<command::Currency>());
  commandCollection.emplace("+доки",     std::make_unique<command::Document>());
  commandCollection.emplace("+офф",      std::make_unique<command::Exit>());
  commandCollection.emplace("+хост",     std::make_unique<command::Host>());
  commandCollection.emplace("+айпи",     std::make_unique<command::Ip>());
  commandCollection.emplace("+кик",      std::make_unique<command::Kick>());
  commandCollection.emplace("+онлайн",   std::make_unique<command::Online>());
  commandCollection.emplace("+пикча",    std::make_unique<command::Picture>());
  commandCollection.emplace("+пинг",     std::make_unique<command::Ping>());
  commandCollection.emplace("+роль",     std::make_unique<command::Role>());
  commandCollection.emplace("+стат",     std::make_unique<command::Stat>());
  commandCollection.emplace("+видео",    std::make_unique<command::Video>());
  commandCollection.emplace("+кто",      std::make_unique<command::Who>());
}

bot::MessageEventHandler::~MessageEventHandler() = default;

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

bool bot::MessageEventHandler::isAdmin(long from_id, long peer_id)
{
  return repository->getRole(from_id, peer_id) == "админ";
}

void bot::MessageEventHandler::process(std::string_view message, long peer_id, long from_id)
{
  if (message.empty()     ) return;
  if (message.at(0) != '+') return;
  if (message == "+помощь") processHelp(peer_id);
  processLogging(message);

  tryProcessCommand(message, peer_id, from_id);
}

void bot::MessageEventHandler::tryProcessCommand(std::string_view message, long peer_id, long from_id)
{
  std::string_view trigger = message.substr(0, message.find(' '));
  if (commandCollection.find(trigger) != commandCollection.end())
  {
    if ((commandCollection[trigger]->access() == static_cast<std::uint8_t>(Access::admin)) && not isAdmin(from_id, peer_id))
    {
      return;
    }
    std::string commandAnswer = commandCollection[trigger]->execute({util::getArgs(message).data(), peer_id, from_id}, {repository.get(), jsonUtils.get(), api.get()});
    if (not commandAnswer.empty()) api->sendMessage(commandAnswer, peer_id);
  }
}
