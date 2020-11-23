#include "Face.hpp"
#include "Curl.hpp"

using std::string;
using std::to_string;
using nlohmann::json;
using bot::command::FaceCommand;

string FaceCommand::description() const
{
  return "случайное стрёмное лицо";
}

string FaceCommand::trigger() const
{
  return "+лицо";
}

string FaceCommand::execute([[maybe_unused]]const CommandParams& inputData)
{
  const string file = "face.jpg";
  json cat_api_response = json::parse(cURL::request("https://faceapi.herokuapp.com/faces?", {{"n", "1"}}));
  if (cat_api_response[0]["image_url"].is_null()) {
    return "Что-то пошло не так.";
  }
_mutex.lock();
  if (cURL::download(cat_api_response[0]["image_url"].get<string>(), file) != 0) {
    return "Ошибка при скачивании файла.";
  }

  json response =
    json::parse(cURL::request(cURL::append_vkurl("photos.getMessagesUploadServer"),
     {{ "access_token", info::access_token     },
      { "peer_id",      to_string(inputData.peer_id)},
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
