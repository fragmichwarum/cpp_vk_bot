#include "VkAPI.hpp"
#include "ChatName.hpp"


constexpr std::string_view bot::command::ChatNameSetter::description() const noexcept
{
  return "установить имя чата";
}

std::string bot::command::ChatNameSetter::execute(const CommandParams& params, const Dependencies& deps)
{
  deps.api->editChat(params.peer_id, params.args);
  return "ok";
}
