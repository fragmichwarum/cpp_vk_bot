#include "Network.hpp"
#include "VkAPI.hpp"
#include "Cat.hpp"

constexpr std::string_view bot::command::Cat::description() const noexcept
{
  return "Рандомный котик";
}

std::string bot::command::Cat::execute(const CommandParams& inputData, const Dependencies& deps)
{
  simdjson::dom::parser parser;
  simdjson::dom::array catAPIArray = parser.parse(deps.net->request("https://api.thecatapi.com/v1/images/search", {}));

  if (catAPIArray.at(0)["url"].is_null()) return "Что-то пошло не так.";

  return deps.api->processAttachmentUploading("photo", "cat.jpg", std::string(catAPIArray.at(0)["url"].get_c_str()), inputData.peer_id);
}
