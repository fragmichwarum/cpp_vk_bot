#include "vk_api.hpp"

using bot::Vk_api;
using namespace bot::cURL;
using namespace bot::util;

using nlohmann::json;
using std::to_string;
using std::string;
using std::map;

void Vk_api::send_message(const string& text, const long& peer_id, const map<string, string>& options)
{
  map<string, string> parameters = {
    { "message",      text               },
    { "peer_id",      to_string(peer_id) },
    { "random_id",    "0"                },
    { "access_token", access_token       },
    { "v",            api_version        },
    { "disable_mentions", "1"            }
  };

  if (options.size() != 0) {
    parameters.merge(map<string, string>{options});
  }

  request(append_vkurl("messages.send"), parameters);
}

static string attachment_type(const string& method)
{
  if (method == "photos.search") {
    return "photo";
  }
  if (method == "video.search") {
    return "video";
  }
  if (method == "docs.search") {
    return "doc";
  }
  return "";
}

std::string Vk_api::media_search(const std::string &method, const std::string &text)
{
  json vkmedia =
    json::parse(request(append_vkurl(method),
      {{ "q",            get_args(text)  },
       { "access_token", user_token      },
       { "v",            api_version     },
       { "count",        "50"            }}));

  string docs;
  json items = vkmedia["response"]["items"];

  if (items.size() == 0) {
    return "";
  }

  for (uint8_t i = 0; i < items.size() && i < 10; i++) {
    long index = rand() % items.size();
    docs +=
      attachment_type(method) +
      to_string(items[index]["owner_id"].get<long>()) + '_' +
      to_string(items[index][      "id"].get<long>()) + ',';
  }

  return docs;
}

std::string Vk_api::kick_user(const long& chat_id, const long& user_id)
{
  json response =
    json::parse(request(append_vkurl("messages.removeChatUser"),
     {{ "chat_id",      to_string(chat_id) },
      { "user_id",      to_string(user_id) },
      { "access_token", access_token       },
      { "random_id",    "0"                },
      { "v",            api_version        }}));

  if (not response["error"].is_null() &&
          response["error"]["error_code"] == 100)
  {
    return "Что-то пошло не так.";
  }

  if (not response["error"].is_null() &&
          response["error"]["error_code"] == 15)
  {
    return "Не могу кикнуть этого юзера/группу.";
  }

  return "Arbeit macht frei.";
}
