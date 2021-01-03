#include "bot/include/3rd_party/string_utils.hpp"
#include "bot/include/commands/kick.hpp"


static long get_user_id(std::string_view screen_name)
{
  return std::stol(screen_name.substr(3, 9).data());
}

void bot::command::kick::execute(const vk::event::message_new& event) const
{
  if (event.has_reply())
  {
    messages.remove_chat_user(event.peer_id() - 2000000000, event.reply().from_id());
  }
  else
  {
    std::string args = third_party::string_utils::cut_first(event.text());

    if (args.empty()) { messages.send(event.peer_id(), "Укажи айди участника или перешли его сообщение."); return; }

    messages.remove_chat_user(event.peer_id() - 2000000000, get_user_id(args));
  }
}
