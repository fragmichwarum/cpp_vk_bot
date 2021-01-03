#include "lib/include/methods/docs.hpp"
#include "lib/include/methods/messages.hpp"

#include "bot/include/3rd_party/string_utils.hpp"
#include "bot/include/commands/docs.hpp"


bot::command::docs::~docs() = default;

bot::command::docs::docs()
  : doc(std::make_unique<vk::docs>())
{ }

void bot::command::docs::execute(const vk::event::message_new& event) const
{
  std::string args = third_party::string_utils::cut_first(event.text());

  if (args.empty()) { messages.send(event.peer_id(), "Задана пустая строка."); return; }

  auto docs_list = doc->search(args, 25);

  if (docs_list.empty())
  {
    messages.send(event.peer_id(), "Не найдено документов.");
  }
  else
  {
    messages.send(event.peer_id(), "", docs_list);
  }
}
