#include <simdjson.h>

#include "lib/include/lib/network.hpp"
#include "lib/include/methods/photos.hpp"

#include "../bot/include/commands/cat.hpp"


void bot::command::cat::execute(const vk::event::message_new& event) const
{
  static vk::lib::network network;
  static vk::photos       photos;

  static
  simdjson::dom::parser parser;
  simdjson::dom::array  response =
  parser.parse(network.request("https://api.thecatapi.com/v1/images/search", {}));

  if (network.download("cat.jpg", response.at(0)["url"]) != 0)
  {
    messages.send(event.peer_id(), "Ошибка загрузки.");
    return;
  }

  if (response.at(0)["url"].is_null()) { messages.send(event.peer_id(), "Что-то пошло не так."); return; }

  std::string raw_json = photos.get_messages_upload_server(event.peer_id());
  auto photo = photos.save_messages_photo("cat.jpg", raw_json);

  messages.send(event.peer_id(), "", { photo });
}
