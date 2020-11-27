#include "Curl.hpp"
#include "VkAPI.hpp"
#include "Currency.hpp"

extern template class nlohmann::basic_json<>;

using nlohmann::json;
using bot::command::CurrencyCommand;

const std::string CurrencyCommand::description() const
{
  return "показать курс валют";
}

const std::string CurrencyCommand::trigger() const
{
  return "+курс";
}

const std::vector<std::string> currency_list = {
  "GBP",
  "BYN",
  "USD",
  "EUR",
  "KZT",
  "PLN",
  "UAH",
  "JPY"
};

CurrencyCommand::CurrencyCommand()
  : lastUpdateTime(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count())
{ }

void CurrencyCommand::tryCache(const uint64_t& updateInterval)
{
  unsigned long timeNow = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  bool requestCondition = false;
  requestCondition |= cachedCurrency.is_null();
  requestCondition |= timeNow - lastUpdateTime > updateInterval;
  lastUpdateTime = timeNow;

  if (requestCondition) {
    cachedCurrency = json::parse(cURL::request("https://www.cbr-xml-daily.ru/daily_json.js", {}));
  }
}

const std::string CurrencyCommand::execute([[maybe_unused]] const CommandParams& inputData)
{
  tryCache(600);
  std::string result;
  result += "Курс валют:\n";
  for (const std::string& currency : currency_list) {
    result += std::to_string(cachedCurrency["Valute"][currency]["Nominal"].get<long>());
    result += ' ';
    result += cachedCurrency["Valute"][currency]["Name"];
    result += " -> ";
    result += std::to_string(cachedCurrency["Valute"][currency]["Value"].get<double>()) + "₽\n";
  }
  return result;
}
