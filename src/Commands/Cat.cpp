#include "Info.hpp"
#include "VkAPI.hpp"
#include "Curl.hpp"
#include "Cat.hpp"

extern template class nlohmann::basic_json<>;

using nlohmann::json;
using bot::command::CatCommand;

std::string CatCommand::description() const
{
  return "Рандомный котик";
}

std::string CatCommand::trigger() const
{
  return "+котик";
}

std::string CatCommand::execute([[maybe_unused]]const CommandParams& inputData)
{
  std::string file = "face.jpg";
  json face_api_response = json::parse(cURL::request("https://api.thecatapi.com/v1/images/search", {}));
  if (face_api_response[0]["url"].is_null()) {
    return "Что-то пошло не так.";
  }
_mutex.lock();
  if (cURL::download(face_api_response[0]["url"].get<std::string>(), file) != 0) {
    return "Ошибка при скачивании файла.";
  }

  json response =
    json::parse(cURL::request(cURL::appendVkUrl("photos.getMessagesUploadServer"),
     {{ "access_token", info::accessToken  },
      { "peer_id",      std::to_string(inputData.peer_id) },
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
