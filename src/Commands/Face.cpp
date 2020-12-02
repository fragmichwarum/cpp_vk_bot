#include <simdjson.h>

#include "Face.hpp"
//#include "Curl.hpp"
#include "Info.hpp"
//#include "VkAPI.hpp"

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
  simdjson::dom::parser parser;
  simdjson::dom::array faceAPIArray = parser.parse(cURL::request("https://faceapi.herokuapp.com/faces?", {{"n", "1"}}));

  if (faceAPIArray.at(0)["image_url"].is_null()) {
    return "Что-то пошло не так.";
  }
  return api::processAttachmentUploading("photo", "face.jpg", std::string(faceAPIArray.at(0)["image_url"].get_c_str()), inputData.peer_id);
}
