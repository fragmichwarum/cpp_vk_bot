#include "Utils.hpp"
#include "Invoker.hpp"

using std::string;
using std::vector;
using nlohmann::json;

void bot::Invoker::initCommand(bot::ICommand* command)
{
  commands.emplace_back(command);
}

void bot::Invoker::tryExecute(const json& response)
{
  string responseType = response["type"];

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

void bot::Invoker::processMessageEvent(const json& response)
{
  string message = response["object"]["message"]["text"].get<string>();
  long peer_id = response["object"]["message"]["peer_id"].get<long>();

  if (not message.empty())
  {
    vector<string> args = util::split(message);
    for (const auto& command : commands)
    {
      if (command->trigger() == args[0])
      {
        vkapi::send_message(command->execute({util::getArgs(message), peer_id}), peer_id);
        return;
      }
    }
  }
}

void bot::Invoker::processNewPostEvent(const json& response)
{
  return; // ...
}

std::string bot::Invoker::generateHelp()
{
  std::string help;
  for (const auto& command : commands)
  {
    help += command->trigger();
    help += " - ";
    help += command->description();
    help += '\n';
  }
  return help;
}
