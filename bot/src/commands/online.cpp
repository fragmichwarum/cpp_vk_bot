#include "bot/include/commands/online.hpp"


void bot::command::online::execute(const vk::event::message_new& event) const
{
  vk::conversation_member_list members = messages.get_conversation_members(event.peer_id());

  if (members.empty()) { messages.send(event.peer_id(), "Кажется, сейчас в сети никого нет."); return; }

  std::string online_users = "Список участников онлайн:\n";
  for (auto&& member : members)
  {
    if (member.online)
    {
      online_users += "@id" + std::to_string(member.id) + '(' + member.first_name + ' ' + member.last_name + ")\n";
    }
  }

  messages.send(event.peer_id(), online_users);
}
