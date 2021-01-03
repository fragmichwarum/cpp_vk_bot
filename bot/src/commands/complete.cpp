#include <simdjson.h>

#include "lib/include/lib/network.hpp"

#include "bot/include/3rd_party/string_utils.hpp"
#include "bot/include/commands/complete.hpp"


void bot::command::complete::execute(const vk::event::message_new& event) const
{
  static vk::lib::network network;

  std::string response =
  network.request_data(
    "https://pelevin.gpt.dobro.ai/generate/",
    third_party::string_utils::to_json({{"prompt", event.text()}, {"length", "50"}})
  );

  static
  simdjson::dom::parser parser;
  simdjson::dom::object object = parser.parse(response);

  if (object.begin().key() != "replies")
  {
    messages.send(event.peer_id(), "Ошибка генерации текста");
    return;
  }

  messages.send(
    event.peer_id(),
    third_party::string_utils::cut_first(event.text()) +
    static_cast<std::string>(object["replies"].at(0))
  );
}
