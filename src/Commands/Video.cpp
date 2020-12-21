#include "Utility.hpp"
#include "VkAPI.hpp"
#include "Video.hpp"

constexpr std::string_view bot::command::Video::description() const noexcept
{
  return "поиск видеозаписей ВК";
}

std::string bot::command::Video::execute(const CommandParams& inputData, const Dependencies& deps)
{
  if (inputData.args.empty()) return util::emptyArgs();

  std::string attachments = deps.api->searchMedia("video.search", inputData.args);
  if (attachments.empty()) return "Не найдено видеозаписей.";

  deps.api->sendMessage("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
