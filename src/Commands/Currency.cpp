#include <simdjson.h>
#include <chrono>

#include "Network.hpp"
#include "Currency.hpp"

constexpr std::string_view bot::command::Currency::description() const noexcept
{
  return "показать курс валют";
}

simdjson::dom::object currencies;

uint64_t lastUpdateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
bool isEmptyResponse = true;

const std::vector<std::string> currencyList = {
  "GBP",
  "BYN",
  "USD",
  "EUR",
  "KZT",
  "PLN",
  "UAH",
  "JPY"
};

void tryCache(const uint64_t& interval, const bot::Network& net)
{
  unsigned long now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  bool condition = false;
  condition |= isEmptyResponse;
  condition |= now - lastUpdateTime > interval;
  lastUpdateTime = now;
  isEmptyResponse = false;

  simdjson::dom::parser parser;
  if (condition) currencies = parser.parse(net.request("https://www.cbr-xml-daily.ru/daily_json.js", {}));
}

std::string bot::command::Currency::execute([[maybe_unused]] const CommandParams& params, const Dependencies& deps)
{
  tryCache(600, *deps.net);

  std::string list = "Курс валют:\n";

  for (const std::string& currency : currencyList)
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
