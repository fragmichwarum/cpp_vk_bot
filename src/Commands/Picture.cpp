#include "Picture.hpp"
#include "Utils.hpp"

using std::string;
using bot::command::PictureCommand;

string PictureCommand::description() const
{
  return "поиск картинок ВК";
}

string PictureCommand::trigger() const
{
  return "+пикча";
}

string PictureCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  string attachments = vkapi::media_search("photos.search", inputData.args);
  if (attachments.empty()) {
    return "";
  }
  vkapi::send_message("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
