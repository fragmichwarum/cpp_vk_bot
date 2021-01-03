#include <simdjson.h>

#include "lib/include/lib/common_error.hpp"
#include "lib/include/lib/network.hpp"
#include "lib/include/methods/messages.hpp"

static void append_attachments(std::map<std::string, std::string>& parameters, const vk::attachment::attachments_t& list)
{
  std::string attachments;
  for (auto&& attachment : list)
  {
    attachments += attachment->value() + ',';
  }
  parameters.insert({{"attachment", attachments}});
}

void vk::messages::send(std::int64_t peer_id, std::string_view text, const vk::attachment::attachments_t& list) const
{
  std::map<std::string, std::string> parameters =
  group_params({
    { "random_id",    "0"          },
    { "message",      text.data()  },
    { "peer_id",      std::to_string(peer_id) },
    { "disable_mentions", "1"      },
  });

  if (not list.empty())
  {
    append_attachments(parameters, list);
  }
  call("messages.send", parameters);
}

void vk::messages::send(std::int64_t peer_id, std::string_view text) const
{
  send(peer_id, text, {});
}

void vk::messages::remove_chat_user(std::int64_t chat_id, std::int64_t user_id) const
{
  std::string response
  = call("messages.removeChatUser", group_params({
    { "chat_id",      std::to_string(chat_id) },
    { "user_id",      std::to_string(user_id) },
    { "random_id",    "0" }
  }));

  simdjson::dom::object parsed = parser->parse(response);

  if (parsed.begin().key() == "response" && static_cast<long>(parsed["response"]) == 1) return;

  if (not parsed["error"].is_null() && static_cast<std::int64_t>(parsed["error"]["error_code"]) == 100)
  {
    throw vk::common_error(__FILE__, __LINE__, 100, "Can't kick this user/group.");
  }
  if (not parsed["error"].is_null() && static_cast<std::int64_t>(parsed["error"]["error_code"]) == 15)
  {
    throw vk::common_error(__FILE__, __LINE__, 15, "Access denied.");
  }
}

void vk::messages::edit_chat(std::int64_t chat_id, std::string_view new_title) const
{
  call("messages.editChat", group_params({
    { "chat_id",   std::to_string(chat_id - 2000000000) },
    { "title",     new_title.data() },
    { "random_id", "0" }
   }));
}

void vk::messages::set_chat_photo(std::string_view filename, std::string_view raw_server) const
{
  std::string upload_url = static_cast<std::string>(parser->parse(raw_server)["response"]["upload_url"]);

  std::string raw_upload_response = network->upload("file", filename, static_cast<std::string>(upload_url));

  simdjson::dom::object upload_response = parser->parse(raw_upload_response);

  std::string raw_vk_response =
  call("messages.setChatPhoto", group_params({
    { "file", static_cast<std::string>(upload_response["response"])}
  }));
}

vk::conversation_member_list vk::messages::get_conversation_members(int64_t peer_id) const
{
  std::string raw_response =
  call("messages.getConversationMembers", group_params({
    { "peer_id", std::to_string(peer_id) }
  }));

  simdjson::dom::object response = parser->parse(raw_response);

  if (response.begin().key() == "error" && response["error"]["error_code"].get_int64() == 917L)
  {
    throw common_error(__FILE__, __LINE__, 917, "Access denied.");
  }

  conversation_member_list members;
  for (auto&& profile : response["response"]["profiles"].get_array())
  {
    members.push_back({
      static_cast<std::string_view>(profile["first_name"]).data(),
      static_cast<std::string_view>(profile["last_name"]).data(),
      static_cast<std::int64_t>(profile["id"]),
     (static_cast<std::int64_t>(profile["online"]) == 1) ? true : false
    });
  }

  return members;
}
