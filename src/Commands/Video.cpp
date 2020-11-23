#include "Video.hpp"
#include "Utils.hpp"

using std::string;
using bot::command::VideoCommand;

string VideoCommand::description() const
{
  return "поиск видео ВК";
}

string VideoCommand::trigger() const
{
  return "+видео";
}

string VideoCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  string attachments = vkapi::media_search("video.search", inputData.args);
  if (attachments.empty()) {
    return "";
  }
  vkapi::send_message("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
