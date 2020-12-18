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

bot::VkAPI*   bot::MessageEventHandler::api_                  = new bot::VkAPI;
bot::Network* bot::MessageEventHandler::net_                  = new bot::Network;
bot::Logger*  bot::MessageEventHandler::logger_               = new bot::Logger(api_->getLogPath(), api_->getErrPath());
bot::SQLiteRepository* bot::MessageEventHandler::repository_  = new bot::SQLiteRepository("USERS");

bot::MessageEventHandler::MessageEventHandler()
{
  commandCollection_.emplace("+оботе",    std::make_shared<command::About>());
  commandCollection_.emplace("+котик",    std::make_shared<command::Cat>());
  commandCollection_.emplace("+чатнейм",  std::make_shared<command::ChatNameSetter>());
  commandCollection_.emplace("+дополни",  std::make_shared<command::Complete>());
  commandCollection_.emplace("+курс",     std::make_shared<command::Currency>());
  commandCollection_.emplace("+доки",     std::make_shared<command::Document>());
  commandCollection_.emplace("+хост",     std::make_shared<command::Host>());
  commandCollection_.emplace("+айпи",     std::make_shared<command::Ip>());
  commandCollection_.emplace("+онлайн",   std::make_shared<command::Online>());
  commandCollection_.emplace("+пикча",    std::make_shared<command::Picture>());
  commandCollection_.emplace("+роль",     std::make_shared<command::Role>());
  commandCollection_.emplace("+стат",     std::make_shared<command::Stat>());
  commandCollection_.emplace("+видео",    std::make_shared<command::Video>());
  commandCollection_.emplace("+кто",      std::make_shared<command::Who>());
}

void bot::MessageEventHandler::processLogging_(const std::string& message)
{
  logger_->log(message);
  logger_->print(message);
}

void bot::MessageEventHandler::processHelp_(const long& peer_id)
{
  std::string help;
  for (const std::pair<const std::string, std::shared_ptr<ICommand>>& command : commandCollection_)
  {
    help.append(command.first);
    help.append(" - ");
    help.append(command.second->description());
    help.append("\n");
  }
  api_->sendMessage(help, peer_id);
}

void bot::MessageEventHandler::tryProcessMessage(const simdjson::dom::object& update)
{
  std::string message(update["object"]["message"]["text"].get_c_str());
  long peer_id(update["object"]["message"]["peer_id"].get_int64());

  if (message.empty()) return;
  if (message.at(0) != '+') return;
  if (message == "+помощь") processHelp_(peer_id);

  processLogging_(message);

  for (const std::pair<const std::string, std::shared_ptr<ICommand>>& command : commandCollection_)
  {
    if (command.first == message.substr(0, message.find_first_of(" ")))
    {
      api_->sendMessage(command.second->execute({util::getArgs(message), peer_id}, {repository_, api_, net_}), peer_id);
      return;
    }
  }
}

bot::MessageEventHandler::~MessageEventHandler()
{
  delete api_;
  delete logger_;
  delete repository_;
  delete net_;
}
