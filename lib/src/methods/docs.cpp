#include <simdjson.h>

#include "lib/include/lib/common_error.hpp"
#include "lib/include/lib/network.hpp"
#include "lib/include/methods/docs.hpp"

vk::attachment::attachments_t vk::docs::search(std::string_view query, std::int64_t count) const
{
  return common_search("docs.search", query, count);
}

std::string vk::docs::get_upload_server(std::int64_t group_id) const
{
  return network->request(append_url("docs.getUploadServer"), group_params({
    { "group_id", std::to_string(group_id)}
  }));
}

std::string vk::docs::get_messages_upload_server(std::string_view type, int64_t peer_id) const
{
  return network->request(append_url("docs.getMessagesUploadServer"), group_params({
    { "peer_id", std::to_string(peer_id)},
    { "type",    type.data()    }
  }));
}

std::shared_ptr<vk::attachment::audio_message_attachment> vk::docs::save_audio_message(std::string_view filename, std::string_view raw_server)
{
  std::string upload_url = static_cast<std::string>(parser->parse(raw_server)["response"]["upload_url"]);

  std::string raw_upload_response = network->upload("file", filename, upload_url);

  simdjson::dom::object upload_response = parser->parse(raw_upload_response);

  if (upload_response.begin().key() != "file")
  {
    throw common_error(__FILE__, __LINE__, -1, "Can't upload file. Maybe is not an mp3 track?");
  }

  std::string file = static_cast<std::string_view>(upload_response["file"]).data();

  if (file == "") return {};

  std::string raw_save_response =
  call("docs.save", group_params({
    { "file", file }, { "title", "voice" }
  }));

  simdjson::dom::object uploaded_doc = parser->parse(raw_save_response)["response"]["audio_message"];

  return
  std::make_shared<vk::attachment::audio_message_attachment>(
    static_cast<std::int64_t>(uploaded_doc["owner_id"]),
    static_cast<std::int64_t>(uploaded_doc["id"]),
    static_cast<std::string_view>(uploaded_doc["link_ogg"]),
    static_cast<std::string_view>(uploaded_doc["link_mp3"])
  );
}
