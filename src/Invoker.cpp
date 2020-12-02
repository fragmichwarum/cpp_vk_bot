#include "Utils.hpp"
#include "Info.hpp"
#include "Invoker.hpp"
#include "../lib/include/crc32.hpp"

using bot::Invoker;

void Invoker::initCommand(bot::ICommand* command)
{
  commands.emplace_back(command);
}

void Invoker::tryExecute(const simdjson::dom::object& response)
{
  long responseHash = crc32gen(response["type"].get_c_str());

  switch (responseHash) {
    case EVENT::MESSAGE_NEW:
      processMessageEvent(response);

    case EVENT::WALL_POST_NEW:
      processNewPostEvent(response);

    default:
      return;
  }
}

void Invoker::processMessageEvent(const simdjson::dom::object& response)
{
  std::string message(response["object"]["message"]["text"].get_c_str());
  long peer_id = response["object"]["message"]["peer_id"].get_int64();

  if (message.empty()) {
    return;
  }
  if (message.at(0) == '+') {
    eventLogger.print(message);
    eventLogger.log(message);
  }
  std::vector<std::string> args = util::split(message);

  for (const auto& command : commands) {
    if (command->trigger() == args[0]) {
      info::processedMessages++;
      api::sendMessage(command->execute({util::getArgs(message), peer_id}), peer_id);
      return;
    }
    if (args[0] == "+помощь") {
      api::sendMessage(generateHelp(), peer_id);
      return;
    }
  }
}

void Invoker::processNewPostEvent([[maybe_unused]] const simdjson::dom::object& response)
{
  return; // ...
}

std::string Invoker::generateHelp()
{
  std::string help = "Список команд:\n";
  for (const auto& command : commands)
  {
    help += command->trigger();
    help += " - ";
    help += command->description();
    help += '\n';
  }
  return help;
}
