#include "Curl.hpp"
#include "Utils.hpp"
#include "Genius.hpp"
#include "VkAPI.hpp"

extern template class nlohmann::basic_json<>;

using bot::command::GeniusCommand;
using nlohmann::json;

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
  json songs =
    json::parse(cURL::request("https://api.genius.com/search?",
     {{ "q",            inputData.args },
      { "access_token", genius_token   }}));

  if (songs["response"]["hits"].size() == 0) {
    return "Кажется такого исполнителя нет.";
  }

  std::string songs_message;
  long max_size = 10;
  for (uint8_t i = 0; i < songs["response"]["hits"].size() && i < max_size; i++) {
    songs_message += songs["response"]["hits"][i]["result"]["full_title"];
    songs_message += "\nСcылка: ";
    songs_message += songs["response"]["hits"][i]["result"]["url"];
    songs_message += "\n\n";
  }
  return songs_message;
}
