#include "Cat.hpp"

using bot::command::Cat;

const std::string Cat::description() const
{
  return "Рандомный котик";
}

const std::string Cat::trigger() const
{
  return "+котик";
}

const std::string Cat::execute([[maybe_unused]]const CommandParams& inputData)
{
  simdjson::dom::array catAPIArray = parser.parse(net->request("https://api.thecatapi.com/v1/images/search", {}));

  if (catAPIArray.at(0)["image_url"].is_null()) {
    return "Что-то пошло не так.";
  }
  return api->processAttachmentUploading("photo", "cat.jpg", std::string(catAPIArray.at(0)["url"].get_c_str()), inputData.peer_id);
}
