#include "Video.hpp"
#include "Utils.hpp"
#include "VkAPI.hpp"

using bot::command::VideoCommand;

const std::string VideoCommand::description() const
{
  return "поиск видео ВК";
}

const std::string VideoCommand::trigger() const
{
  return "+видео";
}

const std::string VideoCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  std::string attachments = api::media_search("video.search", inputData.args);
  if (attachments.empty()) {
    return "";
  }
  api::send_message("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
