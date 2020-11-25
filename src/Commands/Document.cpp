#include "Utils.hpp"
#include "VkAPI.hpp"
#include "Document.hpp"

using bot::command::DocumentCommand;

std::string DocumentCommand::description() const
{
  return "поиск документов ВК";
}

std::string DocumentCommand::trigger() const
{
  return "+доки";
}

std::string DocumentCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  std::string attachments = vkapi::media_search("docs.search", inputData.args);
  if (attachments.empty()) {
    return "";
  }
  vkapi::send_message("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
