#include <simdjson.h>

#include "Face.hpp"
#include "Curl.hpp"
#include "Info.hpp"
#include "VkAPI.hpp"

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
  std::string file = "face.jpg";
  std::string catResponse = cURL::request("https://faceapi.herokuapp.com/faces?", {{"n", "1"}});

  simdjson::padded_string padded_string = catResponse;
  simdjson::dom::parser parser;
  simdjson::dom::array faceAPIArray = parser.parse(padded_string);

  if (faceAPIArray.at(0)["image_url"].is_null()) {
    return "Что-то пошло не так.";
  }

_mutex.lock();
  if (cURL::download(std::string{faceAPIArray.at(0)["image_url"].get_c_str()}, file) != 0) {
    return "Ошибка при скачивании файла.";
  }

  std::string uploadServer =
    cURL::request(cURL::appendVkUrl("photos.getMessagesUploadServer"),
     {{ "access_token", info::accessToken  },
      { "peer_id",      std::to_string(inputData.peer_id) },
      { "album_id",     "0"                },
      { "group_id",     info::groupId      },
      { "v",            info::version      }
     });

  padded_string = uploadServer;
  simdjson::dom::object uploadServerObject = parser.parse(uploadServer);

  using owner_id = long;
  using       id = long;

  std::pair<owner_id, id> attachment
    = api::upload_attachment("photo", file, std::string{uploadServerObject["response"]["upload_url"].get_c_str()});
_mutex.unlock();

  api::send_message("", inputData.peer_id, {{"attachment", "photo" + std::to_string(attachment.first)
                          + "_" + std::to_string(attachment.second)}});
  return "";
}
