#pragma once

#include <map>
#include "lib/include/api/base_object.hpp"
#include "lib/include/attachment/attachment.hpp"

namespace vk
{
struct conversation_member
{
  std::string first_name;
  std::string last_name;
  std::int64_t id;
  bool online;
};
using conversation_member_list = std::list<conversation_member>;
}

namespace vk
{
class messages : private base_object
{
public:
  void send(std::int64_t peer_id, std::string_view text) const;
  void send(std::int64_t peer_id, std::string_view text, const attachment::attachments_t& list) const;
  void remove_chat_user(std::int64_t chat_id, std::int64_t user_id) const;
  void edit_chat(std::int64_t chat_id, std::string_view new_title) const;
  void set_chat_photo(std::string_view filename, std::string_view raw_server) const;
  conversation_member_list get_conversation_members(std::int64_t peer_id) const;
};
}
