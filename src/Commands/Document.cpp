#include "Document.hpp"

const std::string bot::command::Document::description() const
{
  return "поиск документов ВК";
}

const std::string bot::command::Document::trigger() const
{
  return "+доки";
}

const std::string bot::command::Document::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  std::string attachments = api->searchMedia("docs.search", inputData.args);
  if (attachments.empty()) {
    return "Не найдено документов.";
  }
  api->sendMessage("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
