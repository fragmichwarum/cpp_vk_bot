#include "Picture.hpp"
#include "Utils.hpp"
#include "VkAPI.hpp"

using bot::command::PictureCommand;

std::string PictureCommand::description() const
{
  return "поиск картинок ВК";
}

std::string PictureCommand::trigger() const
{
  return "+пикча";
}

std::string PictureCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  std::string attachments = vkapi::media_search("photos.search", inputData.args);
  if (attachments.empty()) {
    return "";
  }
  vkapi::send_message("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
