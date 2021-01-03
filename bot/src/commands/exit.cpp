#include "bot/include/commands/exit.hpp"


void bot::command::exit::execute(const vk::event::message_new&) const
{
  std::exit(0);
}
