#include "JsonUtils.hpp"
#include "Cat.hpp"

constexpr std::uint8_t bot::command::Cat::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::Cat::description() const noexcept
{
  return "Рандомный котик";
}

std::string bot::command::Cat::execute(const CommandParams& params, const Dependencies& deps)
{
  return deps.jsonUtils->uploadCatImage(params.peer_id);
}
