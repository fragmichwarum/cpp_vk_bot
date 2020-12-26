#include "Exit.hpp"

constexpr uint8_t bot::command::Exit::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::admin);
}

constexpr std::string_view bot::command::Exit::description() const noexcept
{
  return "выключить бота";
}

std::string bot::command::Exit::execute(const CommandParams&, const Dependencies&)
{
  exit(0);
}
