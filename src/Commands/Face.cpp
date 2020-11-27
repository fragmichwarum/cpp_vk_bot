#include "Face.hpp"
#include "Curl.hpp"
#include "Info.hpp"
#include "VkAPI.hpp"

extern template class nlohmann::basic_json<>;

using nlohmann::json;
using bot::command::FaceCommand;

const std::string FaceCommand::description() const
{
  return "случайное стрёмное лицо";
}

const std::string FaceCommand::trigger() const
{
  return "+лицо";
}

const std::string FaceCommand::execute([[maybe_unused]]const CommandParams& inputData)
{
  const std::string file = "face.jpg";
  json cat_api_response = json::parse(cURL::request("https://faceapi.herokuapp.com/faces?", {{"n", "1"}}));
  if (cat_api_response[0]["image_url"].is_null()) {
    return "Что-то пошло не так.";
  }
_mutex.lock();
  if (cURL::download(cat_api_response[0]["image_url"].get<std::string>(), file) != 0) {
    return "Ошибка при скачивании файла.";
  }

  json response =
    json::parse(cURL::request(cURL::appendVkUrl("photos.getMessagesUploadServer"),
     {{ "access_token", info::accessToken  },
      { "peer_id",      std::to_string(inputData.peer_id)},
      { "album_id",     "0"                },
      { "group_id",     info::groupId      },
      { "v",            info::version      }
     }));

  using owner_id = long;
  using       id = long;

  std::pair<owner_id, id> attachment = vkapi::upload_attachment("photo", file, response["response"]["upload_url"]);
_mutex.unlock();

  vkapi::send_message("", inputData.peer_id, {{"attachment", "photo" + std::to_string(attachment.first)
                          + "_" + std::to_string(attachment.second)}});
  return "";
}
