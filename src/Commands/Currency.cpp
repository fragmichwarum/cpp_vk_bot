#include "JsonUtils.hpp"
#include "Currency.hpp"

constexpr std::uint8_t bot::command::Currency::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::Currency::description() const noexcept
{
  return "показать курс валют";
}

std::string bot::command::Currency::execute(const CommandParams& /* unused */, const Dependencies& /* unused */)
{
  return jsonUtils::getCurrencyList();
}
