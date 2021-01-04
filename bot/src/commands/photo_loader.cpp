#include <curlpp/Exception.hpp>

#include "lib/include/lib/common_error.hpp"
#include "lib/include/lib/network.hpp"
#include "lib/include/methods/photos.hpp"

#include "../bot/include/3rd_party/string_utils.hpp"
#include "../bot/include/commands/photo_loader.hpp"


static std::size_t download(vk::lib::network& network, std::string_view filename, std::string_view server)
{
  std::size_t error_code;
  try
  {
    error_code = network.download(filename, server);
  }
  catch (curlpp::LibcurlRuntimeError&)
  {
    return 418;
  }

  return error_code;
}

static void upload(std::string_view filename, const vk::photos& photos, const vk::messages& messages, const vk::event::message_new& event)
{
  std::string raw_json = photos.get_messages_upload_server(event.peer_id());
  std::shared_ptr<vk::attachment::photo_attachment> photo;
  try
  {
    photo = photos.save_messages_photo(filename, raw_json);
  }
  catch (vk::common_error&)
  {
    messages.send(event.peer_id(), "Ошибка загрузки."); return;
  }

  messages.send(event.peer_id(), "", { photo });
}

void bot::command::photo_loader::execute(const vk::event::message_new& event) const
{
  static vk::lib::network network;
  static vk::photos photos;

  std::string args = third_party::string_utils::cut_first(event.text());

  if (args.empty()) { messages.send(event.peer_id(), "Задана пустая строка."); return; }

  if (download(network, "user_picture.jpg", args) == 418)
  {
    messages.send(event.peer_id(), "Ошибка загрузки.");
    return;
  }

  upload("user_picture.jpg", photos, messages, event);
}
