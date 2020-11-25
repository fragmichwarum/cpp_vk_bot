#include "Video.hpp"
#include "Utils.hpp"
#include "VkAPI.hpp"

using bot::command::VideoCommand;

std::string VideoCommand::description() const
{
  return "поиск видео ВК";
}

std::string VideoCommand::trigger() const
{
  return "+видео";
}

std::string VideoCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  std::string attachments = vkapi::media_search("video.search", inputData.args);
  if (attachments.empty()) {
    return "";
  }
  vkapi::send_message("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
