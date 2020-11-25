#include "Weather.hpp"
#include "Utils.hpp"
#include "Curl.hpp"
#include "../lib/include/Json.hpp"

using nlohmann::json;
using bot::command::WeatherCommand;

std::string WeatherCommand::description() const
{
  return "показать погоду.";
}

std::string WeatherCommand::trigger() const
{
  return "+погода";
}

json WeatherCommand::executeQuery(const std::string& inputData)
{
  return
    json::parse(cURL::request("http://api.openweathermap.org/data/2.5/weather?",
     {{ "lang",  "ru"        },
      { "units", "metric"    },
      { "APPID", "ef23e5397af13d705cfb244b33d04561" },
      { "q",      inputData  }}));
}

std::string WeatherCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }

  json weather = executeQuery(inputData.args);

  if (weather["weather"].is_null()) {
    return "Нет такого города.";
  }

  std::string description = weather["weather"][0]["description"];
  int temp           = weather["main"]["temp"];
  int feels_like     = weather["main"]["feels_like"];
  int humidity       = weather["main"]["humidity"];
  std::string city_name = weather["name"];
  return
    "Сейчас в  " + city_name + " " + std::to_string(temp) +
    "°C, " + description + "\nОщущается как " +
    std::to_string(feels_like) + "°C\nВлажность: " +
    std::to_string(humidity) + "%";
}
