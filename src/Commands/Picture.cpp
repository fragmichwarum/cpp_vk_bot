#include "Picture.hpp"

const std::string bot::command::Picture::description() const
{
  return "поиск картинок в ВК";
}

const std::string bot::command::Picture::trigger() const
{
  return "+пикча";
}

const std::string bot::command::Picture::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  std::string attachments = api->searchMedia("photos.search", inputData.args);
  if (attachments.empty()) {
    return "Не найдено картинок.";
  }
  api->sendMessage("", inputData.peer_id, {{"attachment", attachments}});
  return "";
}
