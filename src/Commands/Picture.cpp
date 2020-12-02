#include "Picture.hpp"
#include "Utils.hpp"
//#include "VkAPI.hpp"

using bot::command::PictureCommand;

const std::string PictureCommand::description() const
{
  return "поиск картинок ВК";
}

const std::string PictureCommand::trigger() const
{
  return "+пикча";
}

const std::string PictureCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  std::string attachments = api::mediaSearch("photos.search", inputData.args);
  if (attachments.empty()) {
    return "";
  }
  api::sendMessage("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
