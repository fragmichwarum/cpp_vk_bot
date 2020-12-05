#include "Video.hpp"

const std::string bot::command::Video::description() const
{
  return "поиск видеозаписей ВК";
}

const std::string bot::command::Video::trigger() const
{
  return "+видео";
}

const std::string bot::command::Video::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  std::string attachments = api->searchMedia("video.search", inputData.args);
  if (attachments.empty()) {
    return "Не найдено видеозаписей.";
  }
  api->sendMessage("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
