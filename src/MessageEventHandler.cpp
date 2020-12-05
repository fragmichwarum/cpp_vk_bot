#include "MessageEventHandler.hpp"

void bot::MessageEventHandler::addCommand(bot::ICommand* command)
{
  commandCollection.emplace_back(command);
}

void bot::MessageEventHandler::processLogging(const std::string& message)
{
  logger.log(message);
  logger.print(message);
}

void bot::MessageEventHandler::tryProcessMessage(const simdjson::dom::object& update)
{
  std::string message(update["object"]["message"]["text"].get_c_str());

  if (message.empty()) return;
  if (message.at(0) != '+') return;

  processLogging(message);

  long peerId(update["object"]["message"]["peer_id"].get_int64());

  for (const std::shared_ptr<ICommand>& command : commandCollection)
  {
    if (command->trigger() == message.substr(0, message.find_first_of(" "))) {
      api->sendMessage(command->execute({util::getArgs(message), peerId}), peerId);
      return;
    }
  }
}
