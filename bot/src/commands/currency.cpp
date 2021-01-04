#include <simdjson.h>

#include "lib/include/lib/network.hpp"

#include "../bot/include/commands/currency.hpp"

void bot::command::currency::execute(const vk::event::message_new& event) const
{
  static constexpr const char* codes[] = { "GBP", "BYN", "USD", "EUR", "KZT", "PLN", "UAH", "JPY" };

  static vk::lib::network network;
  static
  simdjson::dom::parser parser;
  simdjson::dom::object object =
  parser.parse(
    network.request("https://www.cbr-xml-daily.ru/daily_json.js", {})
  );

  std::string currencies = "Курс валют:\n";

  for (std::string_view currency : codes)
  {
    currencies += std::to_string(object["Valute"][currency]["Nominal"].get_int64());
    currencies += ' ';
    currencies += object["Valute"][currency]["Name"].get_c_str();
    currencies += " -> ";
    currencies += std::to_string(object["Valute"][currency]["Value"].get_double());
    currencies += "₽\n";
  }

  messages.send(event.peer_id(), currencies);
}
