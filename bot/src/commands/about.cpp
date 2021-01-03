#include "bot/include/commands/about.hpp"


void bot::command::about::execute(const vk::event::message_new& event) const
{
  messages.send(
    event.peer_id(),
    "C++ bot,\n"
    "Cурсы: https://github.com/fragmichwarum/cpp_vk_bot\n"
    "Библиотека: https://github.com/fragmichwarum/cpp_vk_lib\n"
    "Бота создал: @jijijijijijijijijijijijji (он)\n"
    "Собран: " + std::string(__DATE__) + ' ' + std::string(__TIME__)
  );
}
