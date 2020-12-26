#include "Utility.hpp"
#include "Complete.hpp"
#include "JsonUtils.hpp"

constexpr std::uint8_t bot::command::Complete::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::Complete::description() const noexcept
{
  return "дополнить текст";
}

std::string bot::command::Complete::execute(const CommandParams& params, const Dependencies& /* unused */)
{
  if (params.args.empty()) return util::emptyArgs().data();

  return jsonUtils::completeText(params.args);
}
