#include "VkAPI.hpp"
#include "ChatName.hpp"

std::string bot::command::ChatNameSetter::description() const
{
  return "установить имя чата";
}

std::string bot::command::ChatNameSetter::execute(const CommandParams& params, const Dependencies& deps)
{
  deps.api->editChat(params.peer_id, params.args);
  return "ok";
}
