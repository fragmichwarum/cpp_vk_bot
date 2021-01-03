#include "lib/include/methods/photos.hpp"
#include "lib/include/methods/messages.hpp"

#include "bot/include/3rd_party/string_utils.hpp"
#include "bot/include/commands/pics.hpp"


bot::command::pics::~pics() = default;

bot::command::pics::pics()
  : photos(std::make_unique<vk::photos>  ())
{ }

void bot::command::pics::execute(const vk::event::message_new& event) const
{
  std::string args = third_party::string_utils::cut_first(event.text());

  if (args.empty()) { messages.send(event.peer_id(), "Задана пустая строка."); return; }

  auto photos_list = photos->search(args, 25);

  if (photos_list.empty())
  {
    messages.send(event.peer_id(), "Не найдено фотографий");
  }
  else
  {
    messages.send(event.peer_id(), "", photos_list);
  }
}
