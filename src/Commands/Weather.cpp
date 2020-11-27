#include "Weather.hpp"
#include "Utils.hpp"
#include "Curl.hpp"
#include "../lib/include/Json.hpp"

extern template class nlohmann::basic_json<>;

using nlohmann::json;
using bot::command::WeatherCommand;

const std::string WeatherCommand::description() const
{
  return "показать погоду.";
}

const std::string WeatherCommand::trigger() const
{
  return "+погода";
}

bot::traits::dictionary WeatherCommand::generateQuery(const std::string& inputData)
{
  return
   {{ "lang",  "ru"       },
    { "units", "metric"   },
    { "APPID", "ef23e5397af13d705cfb244b33d04561"},
    { "q",      inputData }};
}

const std::string WeatherCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }

  json weather =
      json::parse(cURL::request("http://api.openweathermap.org/data/2.5/weather?", generateQuery(inputData.args)));

  if (weather["weather"].is_null()) {
    return "Нет такого города.";
  }

  std::string description = weather["weather"][0]["description"];
  long temp = weather["main"]["temp"];
  long feels_like = weather["main"]["feels_like"];
  long humidity = weather["main"]["humidity"];
  std::string city_name = weather["name"];
  return
    "Сейчас в  " + city_name + " " + std::to_string(temp) +
    "°C, " + description + "\nОщущается как " +
    std::to_string(feels_like) + "°C\nВлажность: " +
    std::to_string(humidity) + "%";
}
