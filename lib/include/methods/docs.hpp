#pragma once

#include "lib/include/document/common_document.hpp"

namespace vk
{
class docs : private document::common
{
public:
  vk::attachment::attachments_t search(std::string_view query, std::int64_t count) const;
  std::string get_upload_server(std::int64_t group_id) const;
  std::string get_messages_upload_server(std::string_view type, std::int64_t peer_id) const;
  std::shared_ptr<vk::attachment::audio_message_attachment> save_audio_message(std::string_view file, std::string_view raw_server);
};
}
