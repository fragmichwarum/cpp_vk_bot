#include <simdjson.h>

#include "lib/include/lib/common_error.hpp"
#include "lib/include/lib/network.hpp"
#include "lib/include/methods/audio.hpp"

std::string vk::audio::get_upload_server() const
{
  return call("audio.getUploadServer", user_params({}));
}

void vk::audio::save(std::string_view artist, std::string_view title, std::string_view filename, std::string_view raw_server)
{
  std::string raw_upload_response =
  network->upload("file", filename, static_cast<std::string_view>(parser->parse(raw_server)["response"]["upload_url"]));

  simdjson::dom::object upload_response = parser->parse(raw_upload_response);

  if (!upload_response.at_key("error").is_null())
  {
    throw common_error(__FILE__, __LINE__, -1, "Can't upload file. Maybe is not an mp3 track?");
  }

  call("audio.save", user_params({
    { "server",   std::to_string(static_cast<std::int64_t>(upload_response["server"]))},
    { "audio",    static_cast<std::string>(upload_response["audio"])},
    { "hash",     static_cast<std::string>(upload_response["hash"])},
    { "artist",   artist.data()},
    { "title",    title.data()}
  }));
}
