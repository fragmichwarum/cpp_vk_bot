#include "Document.hpp"
#include "Utils.hpp"

using std::string;
using bot::command::DocumentCommand;

string DocumentCommand::description() const
{
  return "поиск документов ВК";
}

string DocumentCommand::trigger() const
{
  return "+доки";
}

string DocumentCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  string attachments = vkapi::media_search("docs.search", inputData.args);
  if (attachments.empty()) {
    return "";
  }
  vkapi::send_message("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
