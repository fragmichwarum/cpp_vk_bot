#include "JsonUtils.hpp"
#include "Online.hpp"

constexpr std::uint8_t bot::command::Online::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::admin);
}

constexpr std::string_view bot::command::Online::description() const noexcept
{
  return "показать участников онлайн";
}

std::string bot::command::Online::execute(const CommandParams& params, const Dependencies& /* unused */)
{
  return jsonUtils::getOnlineUsers(params.peer_id);
}
