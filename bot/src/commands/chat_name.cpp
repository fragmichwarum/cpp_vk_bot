#include "bot/include/3rd_party/string_utils.hpp"

#include "bot/include/commands/chat_name.hpp"


void bot::command::chat_name::execute(const vk::event::message_new& event) const
{
  messages.edit_chat(event.peer_id(), third_party::string_utils::cut_first(event.text()));
}
