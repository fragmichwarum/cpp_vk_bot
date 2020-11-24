#include "Curl.hpp"
#include "Utils.hpp"
#include "Genius.hpp"

using std::string;
using bot::command::GeniusCommand;
using nlohmann::json;

string GeniusCommand::description() const
{
  return "поиск музыки на Genius";
}

string GeniusCommand::trigger() const
{
  return "+трек";
}

string GeniusCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  string genius_token = "JSgH4gUYSn3S2C6Wd4BUhGuV1FWaKSET9DQVl-HBqlqeQ3isoW5bXSllR90VKvQF";
  json songs =
    json::parse(cURL::request("https://api.genius.com/search?",
     {{ "q",            inputData.args },
      { "access_token", genius_token   }}));

  if (songs["response"]["hits"].size() == 0) {
    return "Кажется такого исполнителя нет.";
  }

  string songs_message;
  long resp_size = 0;
  for (json song : songs["response"]["hits"]) {
    if (resp_size++ == 10) {
      break;
    }
    songs_message += song["result"]["full_title"];
    songs_message += "\nСcылка: ";
    songs_message += song["result"]["url"];
    songs_message += "\n\n";
  }
  return songs_message;
}
