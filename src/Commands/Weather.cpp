#include <simdjson.h>

#include "Weather.hpp"
#include "Utils.hpp"
//#include "Curl.hpp"

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

  std::string response =
    cURL::request("http://api.openweathermap.org/data/2.5/weather?", generateQuery(inputData.args));

  simdjson::padded_string padded_string = response;
  simdjson::dom::parser parser;
  simdjson::dom::object parsed = parser.parse(padded_string);

  if (parsed["weather"].is_null()) {
    return "Нет такого города.";
  }

  std::string description(parsed["weather"].at(0)["description"].get_c_str());
  std::string cityName   (parsed["name"].get_c_str());
  double temp            (parsed["main"]["temp"].get_double());
  double feelsLike       (parsed["main"]["feels_like"].get_double());
  long humidity          (parsed["main"]["humidity"].get_int64());

  return
    "Сейчас в  " + cityName + " " + std::to_string(temp) +
    "°C, " + description + "\nОщущается как " +
    std::to_string(feelsLike) + "°C\nВлажность: " +
    std::to_string(humidity) + "%";
}
