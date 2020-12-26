#include "Utility.hpp"
#include "JsonUtils.hpp"
#include "Who.hpp"

constexpr std::uint8_t bot::command::Who::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::Who::description() const noexcept
{
  return "выбрать случайного участника";
}

std::string bot::command::Who::execute(const CommandParams& params, const Dependencies& /* unused */)
{
  if (params.args.empty()) return util::emptyArgs().data();

  return jsonUtils::pickRandomChatUser(params.peer_id) + ' ' + params.args + '.';
}
