#include <simdjson.h>

#include "Network.hpp"
#include "Currency.hpp"


constexpr std::string_view bot::command::Currency::description() const noexcept
{
  return "показать курс валют";
}

static constexpr const char* currencyList[] = {
  "GBP",
  "BYN",
  "USD",
  "EUR",
  "KZT",
  "PLN",
  "UAH",
  "JPY"
};

std::string bot::command::Currency::execute([[maybe_unused]] const CommandParams& params, const Dependencies& deps)
{
  simdjson::dom::parser parser;
  simdjson::dom::object currencies = parser.parse(deps.net->request("https://www.cbr-xml-daily.ru/daily_json.js", {}));

  std::string list = "Курс валют:\n";

  for (std::string_view currency : currencyList)
  {
    list += std::to_string(currencies["Valute"][currency]["Nominal"].get_int64());
    list += ' ';
    list += currencies["Valute"][currency]["Name"].get_c_str();
    list += " -> ";
    list += std::to_string(currencies["Valute"][currency]["Value"].get_double());
    list += "₽\n";
  }

  return list;
}
