#include "Currency.hpp"
#include "../VkAPI.hpp"
#include "../Curl.hpp"

using namespace bot::cURL;

using std::string;
using std::to_string;
using std::vector;
using nlohmann::json;
using bot::command::CurrencyCommand;

string CurrencyCommand::description() const
{
  return "показать курс валют";
}

string CurrencyCommand::trigger() const
{
  return "+курс";
}

static const std::vector<std::string> currency_list = {
  "GBP",
  "BYN",
  "USD",
  "EUR",
  "KZT",
  "PLN",
  "UAH",
  "JPY"
};

string CurrencyCommand::execute([[maybe_unused]] const CommandParams& inputData)
{
  if (cachedCurrency.is_null()) {
    cachedCurrency = json::parse(cURL::request("https://www.cbr-xml-daily.ru/daily_json.js", {}));
  }
  string result;
  result += "Курс валют:\n";
  for (const string& currency : currency_list) {
    result += to_string(cachedCurrency["Valute"][currency]["Nominal"].get<long>());
    result += ' ';
    result += cachedCurrency["Valute"][currency]["Name"];
    result += " -> ";
    result += to_string(cachedCurrency["Valute"][currency]["Value"].get<double>()) + "₽\n";
  }
  return result;
}
