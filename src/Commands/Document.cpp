#include "Utility.hpp"
#include "VkAPI.hpp"
#include "Document.hpp"


constexpr std::string_view bot::command::Document::description() const noexcept
{
  return "поиск документов ВК";
}

std::string bot::command::Document::execute(const CommandParams& inputData, const Dependencies& deps)
{
  if (inputData.args.empty()) return util::emptyArgs().data();

  std::string attachments = deps.api->searchMedia("docs.search", inputData.args);
  if (attachments.empty()) return "Не найдено документов.";

  deps.api->sendMessage("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
