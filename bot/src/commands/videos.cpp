#include "lib/include/methods/video.hpp"
#include "lib/include/methods/messages.hpp"

#include "bot/include/3rd_party/string_utils.hpp"
#include "bot/include/commands/videos.hpp"


bot::command::videos::~videos() = default;

bot::command::videos::videos()
  : messages(std::make_unique<vk::messages>())
  , video   (std::make_unique<vk::video>   ())
{ }

void bot::command::videos::execute(const vk::event::message_new& event) const
{
  std::string args = third_party::string_utils::cut_first(event.text());

  if (args.empty()) { messages->send(event.peer_id(), "Задана пустая строка."); return; }

  auto photos_list = video->search(event.text(), 100);

  if (photos_list.empty())
  {
    messages->send(event.peer_id(), "No photos found");
  }
  else
  {
    messages->send(event.peer_id(), "", photos_list);
  }
}
