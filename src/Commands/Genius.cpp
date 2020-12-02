#include <simdjson.h>

//#include "Curl.hpp"
#include "Utils.hpp"
#include "Genius.hpp"
//#include "VkAPI.hpp"

using bot::command::GeniusCommand;

const std::string GeniusCommand::description() const
{
  return "поиск музыки на Genius";
}

const std::string GeniusCommand::trigger() const
{
  return "+трек";
}

const std::string GeniusCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  std::string genius_token = "JSgH4gUYSn3S2C6Wd4BUhGuV1FWaKSET9DQVl-HBqlqeQ3isoW5bXSllR90VKvQF";
  std::string response =
    cURL::request("https://api.genius.com/search?",
     {{ "q",            inputData.args },
      { "access_token", genius_token   }});

  simdjson::padded_string padded_string = response;
  simdjson::dom::parser parser;
  simdjson::dom::object GeniusAPIObject = parser.parse(padded_string);

  if (GeniusAPIObject["response"]["hits"].get_array().size() == 0) {
    return "Кажется такого исполнителя нет.";
  }

  std::string songs_message;
  long max_size = 10;

  simdjson::dom::array geniusArary = GeniusAPIObject["response"]["hits"].get_array();

  for (uint8_t i = 0; i < geniusArary.size() && i < max_size; i++) {
    songs_message += geniusArary.at(i)["result"]["full_title"];
    songs_message += "\nСcылка: ";
    songs_message += geniusArary.at(i)["result"]["url"];
    songs_message += "\n\n";
  }
  return songs_message;
}
