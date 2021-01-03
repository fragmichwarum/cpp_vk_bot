#pragma once

#include "lib/include/document/common_document.hpp"

namespace vk
{
class photos : private document::common
{
public:
  vk::attachment::attachments_t search(std::string_view query, std::int64_t count) const;
  std::string get_messages_upload_server(std::int64_t peer_id) const;
  std::string get_chat_upload_server(std::int64_t chat_id) const;
  std::shared_ptr<vk::attachment::photo_attachment> save_messages_photo(std::string_view filename, std::string_view raw_server) const;
};
}
