#include <simdjson.h>

#include "Info.hpp"
#include "VkAPI.hpp"
#include "Curl.hpp"
#include "Cat.hpp"

using bot::command::CatCommand;

const std::string CatCommand::description() const
{
  return "Рандомный котик";
}

const std::string CatCommand::trigger() const
{
  return "+котик";
}

const std::string CatCommand::execute([[maybe_unused]]const CommandParams& inputData)
{
  std::string file = "cat.jpg";

  simdjson::padded_string padded_string = cURL::request("https://api.thecatapi.com/v1/images/search", {});
  simdjson::dom::parser parser;
  simdjson::dom::array catAPIArray = parser.parse(padded_string);

  if (catAPIArray.at(0)["url"].is_null()) {
    return "Что-то пошло не так.";
  }

_mutex.lock();
  if (cURL::download(std::string{catAPIArray.at(0)["url"].get_c_str()}, file) != 0) {
    return "Ошибка при скачивании файла.";
  }

  padded_string =
    cURL::request(cURL::appendVkUrl("photos.getMessagesUploadServer"),
     {{ "access_token", info::accessToken  },
      { "peer_id",      std::to_string(inputData.peer_id) },
      { "album_id",     "0"                },
      { "group_id",     info::groupId      },
      { "v",            info::version      }
     });

  simdjson::dom::object uploadServerObject = parser.parse(padded_string);

  using owner_id = long;
  using       id = long;

  std::pair<owner_id, id> attachment
    = api::upload_attachment("photo", file, std::string{uploadServerObject["response"]["upload_url"].get_c_str()});
_mutex.unlock();

  api::send_message("", inputData.peer_id, {{"attachment", "photo" + std::to_string(attachment.first)
                          + "_" + std::to_string(attachment.second)}});
  return "";
}
