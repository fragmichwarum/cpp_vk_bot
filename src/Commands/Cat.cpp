#include "Cat.hpp"
#include "Curl.hpp"

using std::string;
using std::to_string;
using nlohmann::json;
using bot::command::CatCommand;

string CatCommand::description() const
{
  return "Рандомный котик";
}

string CatCommand::trigger() const
{
  return "+котик";
}

string CatCommand::execute([[maybe_unused]]const CommandParams& inputData)
{
  string file = "face.jpg";
  json face_api_response = json::parse(cURL::request("https://api.thecatapi.com/v1/images/search", {}));
  if (face_api_response[0]["url"].is_null()) {
    return "Что-то пошло не так.";
  }
_mutex.lock();
  if (cURL::download(face_api_response[0]["url"].get<string>(), file) != 0) {
    return "Ошибка при скачивании файла.";
  }

  json response =
    json::parse(cURL::request(cURL::append_vkurl("photos.getMessagesUploadServer"),
     {{ "access_token", info::access_token     },
      { "peer_id",      to_string(inputData.peer_id) },
      { "album_id",     "0"                    },
      { "group_id",     info::group_id         },
      { "v",            info::version          }
     }));

  using owner_id = long;
  using       id = long;

  std::pair<owner_id, id> attachment = vkapi::upload_attachment("photo", file, response["response"]["upload_url"]);
_mutex.unlock();

  vkapi::send_message("", inputData.peer_id, {{"attachment", "photo" + to_string(attachment.first)
                          + "_" + to_string(attachment.second)}});
  return "";
}
