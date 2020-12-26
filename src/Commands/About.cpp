#include "About.hpp"

constexpr std::uint8_t bot::command::About::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::About::description() const noexcept
{
  return "показать информацию о боте";
}

std::string bot::command::About::execute(const CommandParams& /* unused */, const Dependencies& /* unused */)
{
  return
    "C++ bot,\n"
    "Cурсы: https://github.com/fragmichwarum/cpp_vk_bot\n"
    "Документация: https://fragmichwarum.github.io/cpp_vk_bot/\n"
    "Бота создал: @jijijijijijijijijijijijji (он)\n"
    "Собран: " + std::string(__DATE__) + ' ' + std::string(__TIME__);
}
