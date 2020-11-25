#include "Utils.hpp"
#include "Info.hpp"
#include "Invoker.hpp"

extern template class nlohmann::basic_json<>;

using nlohmann::json;
using bot::Invoker;

void Invoker::initCommand(bot::ICommand* command)
{
  commands.emplace_back(command);
}

void Invoker::tryExecute(const json& response)
{
  std::string responseType = response["type"];

  if (responseType == "message_new") {
    processMessageEvent(response);
    return;
  }
  if (responseType == "wall_post_new") {
    processNewPostEvent(response);
    return;
  } else {
    return;
  }
}

void Invoker::processMessageEvent(const json& response)
{
  std::string message = response["object"]["message"]["text"].get<std::string>();
  long peer_id = response["object"]["message"]["peer_id"].get<long>();

  if (not message.empty())
  {
    std::vector<std::string> args = util::split(message);
    for (const auto& command : commands)
    {
      if (command->trigger() == args[0])
      {
        info::processedMessages++;
        vkapi::send_message(command->execute({util::getArgs(message), peer_id}), peer_id);
        return;
      }
      else if (args[0] == "+помощь")
      {
        vkapi::send_message(generateHelp(), peer_id);
        return;
      }
    }
  }
}

void Invoker::processNewPostEvent(const json& response)
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
