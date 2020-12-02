#include "Utils.hpp"
//#include "VkAPI.hpp"
#include "Document.hpp"

using bot::command::DocumentCommand;

const std::string DocumentCommand::description() const
{
  return "поиск документов ВК";
}

const std::string DocumentCommand::trigger() const
{
  return "+доки";
}

const std::string DocumentCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  std::string attachments = api::mediaSearch("docs.search", inputData.args);
  if (attachments.empty()) {
    return "";
  }
  api::sendMessage("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
