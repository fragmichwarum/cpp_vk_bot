#include "VkAPI.hpp"
#include "Kick.hpp"

constexpr std::uint8_t bot::command::Kick::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::admin);
}

constexpr std::string_view bot::command::Kick::description() const noexcept
{
  return "кикнуть юзера";
}

std::string bot::command::Kick::execute(const CommandParams& params, const Dependencies& deps)
{
  if (params.args.empty()) return "Что-то пошло не так.";

  return deps.api->kick(params.peer_id, params.from_id);
}
