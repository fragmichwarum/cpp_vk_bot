#include "../cmd_holder.hpp"

namespace {
  using nlohmann::json;
}

void cmd_holder::weather_cmd() {
  if (_message == "+погода") {
    _empty_query();
  } else {
    params body;

    body["lang"]   = "ru";
    body["units"]  = "metric";
    body["APPID"]  = "ef23e5397af13d705cfb244b33d04561";
    body["q"]      = _splitted_message[1];

    json parsed = http_processing("http://api.openweathermap.org/data/2.5/weather?", body);

    if (not parsed["weather"].is_null()) {

      string description  = parsed["weather"][0]["description"];
      int temp            = parsed["main"]["temp"];
      int feels_like      = parsed["main"]["feels_like"];
      int humidity        = parsed["main"]["humidity"];
      string city_name    = parsed["name"];

      string weather = "Сейчас в " + city_name + " " +
           to_string(temp) + "°C," + description + "\nОщущается как " +
           to_string(feels_like)   + "°C\nВлажность: " +
           to_string(humidity)     + "%";

      _message_send(weather, NOT_USE_NICKNAME);
    } else {
      _message_send("Нет такого города.", NOT_USE_NICKNAME);
    }
  }
}
