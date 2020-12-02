#include "Info.hpp"
//#include "VkAPI.hpp"
//#include "Curl.hpp"
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
  simdjson::dom::parser parser;
  simdjson::dom::array catAPIArray = parser.parse(cURL::request("https://api.thecatapi.com/v1/images/search", {}));

  if (catAPIArray.at(0)["image_url"].is_null()) {
    return "Что-то пошло не так.";
  }
  return api::processAttachmentUploading("photo", "cat.jpg", std::string(catAPIArray.at(0)["url"].get_c_str()), inputData.peer_id);
}
