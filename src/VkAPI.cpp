#include <fstream>

#include "Curl.hpp"
#include "Utils.hpp"
#include "VkAPI.hpp"
#include "Info.hpp"

using nlohmann::json;

const json _data = json::parse(std::ifstream{"./init.json"});

      long        bot::info::processedMessages  = 0;
const long        bot::info::adminId            = _data["admin_id"];
const std::string bot::info::groupId            = _data["group_id"];
const std::string bot::info::accessToken        = _data["token"]["access_token"];
const std::string bot::info::userToken          = _data["token"]["user_token"];
const std::string bot::info::version            = _data["api_v"];
const std::string bot::info::errfile            = _data["path"]["err"];
const std::string bot::info::logfile            = _data["path"]["log"];

void bot::vkapi::send_message(const std::string& text, const long& peer_id, const traits::dictionary& options)
{
  traits::dictionary parameters = {
    { "message",      text               },
    { "peer_id",      std::to_string(peer_id) },
    { "random_id",    "0"                },
    { "access_token", info::accessToken  },
    { "v",            info::version      },
    { "disable_mentions", "1"            }
  };

  if (options.size() != 0) {
#if __cplusplus >= 201703L
    parameters.merge(traits::dictionary{options});
#else
    parameters.insert(options.begin(), options.end());
#endif
  }

  cURL::request(cURL::appendVkUrl("messages.send"), parameters);
}

static std::string attachment_type(const std::string& method)
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

std::string bot::vkapi::media_search(const std::string& method, const std::string& text)
{
  json vkmedia =
    json::parse(cURL::request(cURL::appendVkUrl(method),
      {{ "q",            text             },
       { "access_token", info::userToken  },
       { "v",            info::version    },
       { "count",        "50"             }}));

  json items = vkmedia["response"]["items"];

  if (items.size() == 0) {
    return "";
  }

  std::string docs;
  for (uint8_t i = 0; i < items.size() && i < 10; i++) {
    long index = rand() % items.size();
    docs +=
      attachment_type(method) +
      std::to_string(items[index]["owner_id"].get<long>()) + '_' +
      std::to_string(items[index][      "id"].get<long>()) + ',';
  }

  return docs;
}

std::string bot::vkapi::kick_user(const long& chat_id, const long& user_id)
{
  json response =
    json::parse(cURL::request(cURL::appendVkUrl("messages.removeChatUser"),
     {{ "chat_id",      std::to_string(chat_id) },
      { "user_id",      std::to_string(user_id) },
      { "random_id",    "0"                },
      { "access_token", info::accessToken  },
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

std::pair<long, long> bot::vkapi::upload_attachment(const std::string &type, const std::string &file, const std::string &server)
{
  json uploaded_file = json::parse(cURL::upload(file, server));

  std::string url = "https://api.vk.com/method/";
  if (type == "photo") {
    url += "photos.saveMessagesPhoto";
  }

  json saved_vk_attachment =
    json::parse(cURL::request(url + "?",
     {{ "photo",         uploaded_file["photo"] },
      { "server",        std::to_string(uploaded_file["server"].get<long>()) },
      { "hash",          uploaded_file["hash"]  },
      { "v",             info::version          },
      { "access_token",  info::accessToken      }
     }));

  if (not saved_vk_attachment["error"].is_null()) {
    return { 0, 0 };
  }

  return { saved_vk_attachment["response"][0]["owner_id"].get<long>(),
           saved_vk_attachment["response"][0][      "id"].get<long>() };
}
