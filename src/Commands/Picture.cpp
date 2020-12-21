#include "VkAPI.hpp"
#include "Utility.hpp"
#include "Picture.hpp"

constexpr std::string_view bot::command::Picture::description() const noexcept
{
  return "поиск картинок в ВК";
}

std::string bot::command::Picture::execute(const CommandParams& inputData, const Dependencies& deps)
{
  if (inputData.args.empty()) return util::emptyArgs();

  std::string attachments = deps.api->searchMedia("photos.search", inputData.args);
  if (attachments.empty()) return "Не найдено картинок.";

  deps.api->sendMessage("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
