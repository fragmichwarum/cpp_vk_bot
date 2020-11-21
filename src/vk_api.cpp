#include <fstream>

#include "vk_api.hpp"

using bot::Vk_api;
using namespace bot::cURL;
using namespace bot::util;
using namespace bot::info;

using nlohmann::json;
using std::to_string;
using std::string;
using std::map;

const nlohmann::json _data = json::parse(std::ifstream{"./init.json"});

long        bot::info::admin_id     = _data["admin_id"];
std::string bot::info::group_id     = _data["group_id"];
std::string bot::info::access_token = _data["token"]["access_token"];
std::string bot::info::user_token   = _data["token"]["user_token"];
std::string bot::info::version      = _data["api_v"];
std::string bot::info::errfile      = _data["path"]["err"];
std::string bot::info::logfile      = _data["path"]["log"];

void Vk_api::send_message(const string& text, const long& peer_id, const map<string, string>& options)
{
  map<string, string> parameters = {
    { "message",      text               },
    { "peer_id",      to_string(peer_id) },
    { "random_id",    "0"                },
    { "access_token", info::access_token },
    { "v",            info::version      },
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
      {{ "q",            get_args(text)   },
       { "access_token", info::user_token },
       { "v",            info::version    },
       { "count",        "50"             }}));

  json items = vkmedia["response"]["items"];

  if (items.size() == 0) {
    return "";
  }

  string docs;
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
      { "random_id",    "0"                },
      { "access_token", info::access_token },
      { "v",            info::version      }}));

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

std::pair<long, long> Vk_api::upload_attachment(const std::string &type, const std::string &file, const std::string &server)
{
  json uploaded_file = json::parse(cURL::upload(file, server));

  string url = "https://api.vk.com/method/";
  if (type == "photo") {
    url += "photos.saveMessagesPhoto";
  }

  json saved_vk_attachment =
    json::parse(request(url + "?",
     {{ "photo",         uploaded_file["photo"] },
      { "server",        to_string(uploaded_file["server"].get<long>()) },
      { "hash",          uploaded_file["hash"]  },
      { "v",             info::version          },
      { "access_token",  info::access_token     }
     }));

  return { saved_vk_attachment["response"][0]["owner_id"].get<long>(),
           saved_vk_attachment["response"][0][      "id"].get<long>() };
}
