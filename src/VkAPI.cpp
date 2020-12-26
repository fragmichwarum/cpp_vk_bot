#include <simdjson.h>

#include "Network.hpp"
#include "VkAPI.hpp"
#include "VKError.hpp"


namespace
{
/*!
 * @brief The @ref bot::VkAPI private fields initializer.
 *
 * All functions marked as `noexcept` due to the better object code optimization.
 * There is no reason to handle exceptions from these functions.
 * The program should crash if the unsuccessful attempt to open the config file was occured.
 */
class _VkConfig
{
private:
  friend class bot::VkAPI;
  simdjson::dom::parser parser_;
  simdjson::dom::element element_;

  vk_always_inline _VkConfig(std::string_view file)         noexcept { element_ = parser_.load(file.data()); }
  vk_always_inline std::string_view loadAccessToken() const noexcept { return static_cast<const char*>(element_["token"]["access_token"]); }
  vk_always_inline std::string_view loadUserToken()   const noexcept { return static_cast<const char*>(element_["token"]["user_token"]); }
  vk_always_inline std::string_view loadApiVersion()  const noexcept { return static_cast<const char*>(element_["api_v"]); }
  vk_always_inline std::string_view loadGroupId()     const noexcept { return static_cast<const char*>(element_["group_id"]); }
  vk_always_inline std::string_view loadLogPath()     const noexcept { return static_cast<const char*>(element_["path"]["log"]); }
  vk_always_inline std::string_view loadErrorPath()   const noexcept { return static_cast<const char*>(element_["path"]["err"]); }
};
}

bot::Network* bot::VkAPI::net = new bot::Network;
simdjson::dom::parser parser;

bot::VkAPI::VkAPI(const std::string& path)
{
  _VkConfig config(path);

  accessToken_  = config.loadAccessToken();
  userToken_    = config.loadUserToken();
  groupId_      = config.loadGroupId();
  apiVersion_   = config.loadApiVersion();
  logPath_      = config.loadLogPath();
  errPath_      = config.loadErrorPath();
}

bot::VkAPI::~VkAPI()
{
  delete net;
}

std::string bot::VkAPI::appendVkUrl_(std::string_view method)
{
  return "https://api.vk.com/method/" + std::string { method.data() } + '?';
}

bot::LongPollData bot::VkAPI::getLongPollServer()
{
  std::string response =
    net->request(appendVkUrl_("groups.getLongPollServer"),
     {{ "group_id",     groupId_      },
      { "random_id",    "0"           },
      { "access_token", accessToken_  },
      { "v",            apiVersion_   }});

  simdjson::dom::object parsed = parser.parse(response);

  if (parsed.begin().key() == "error") throw Vk_exception(parsed["error"]["error_code"].get_int64());

  return {
    std::string { parser.parse(response)["response"]["key"].get_c_str()    },
    std::string { parser.parse(response)["response"]["server"].get_c_str() },
    std::string { parser.parse(response)["response"]["ts"].get_c_str()     }
  };
}

std::string bot::VkAPI::listenLongPoll(std::string_view key, std::string_view server, std::string_view ts)
{
  return net->request(std::string { server.data() } + '?', {{"act", "a_check"}, {"key", key.data()}, {"ts", ts.data()}, {"wait", "90"}});
}

void bot::VkAPI::sendMessage(std::string_view text, long peer_id, const std::map<std::string, std::string>& options)
{
  std::map<std::string, std::string> parameters = {
    { "message",      text.data()  },
    { "peer_id",      std::to_string(peer_id) },
    { "random_id",    "0"          },
    { "access_token", accessToken_ },
    { "v",            apiVersion_  },
    { "disable_mentions", "1"      }
  };

  if (options.size() != 0) parameters.insert(options.begin(), options.end());

  net->request(appendVkUrl_("messages.send"), parameters);
}

std::string bot::VkAPI::kick(long chat_id, long user_id)
{
  std::string response =
    net->request(appendVkUrl_("messages.removeChatUser"),
     {{ "chat_id",      std::to_string(chat_id) },
      { "user_id",      std::to_string(user_id) },
      { "random_id",    "0"          },
      { "access_token", accessToken_ },
      { "v",            apiVersion_  }});

  simdjson::dom::object parsed = parser.parse(response);

  if (not parsed["error"].is_null() && parsed["error"]["error_code"].get_int64() == 100)
  {
    return "Что-то пошло не так.";
  }

  if (not parsed["error"].is_null() && parsed["error"]["error_code"].get_int64() == 15)
  {
    return "Не могу кикнуть этого юзера/группу.";
  }

  return "Arbeit macht frei.";
}

std::string bot::VkAPI::getConversationMembers(long peer_id)
{
  return
    net->request(appendVkUrl_("messages.getConversationMembers"),
     {{ "fields",       "online"      },
      { "peer_id",      std::to_string(peer_id) },
      { "random_id",    "0"           },
      { "access_token", accessToken_  },
      { "v",            apiVersion_   }});
}

void bot::VkAPI::editChat(long chat_id, std::string_view title)
{
  net->request(appendVkUrl_("messages.editChat"),
   {{ "chat_id",      std::to_string(chat_id - 2000000000) },
    { "title",        title.data() },
    { "random_id",    "0"          },
    { "access_token", accessToken_ },
    { "v",            apiVersion_  }});
}

std::pair<long, long> bot::VkAPI::uploadAttachment_(std::string_view type, std::string_view file, std::string_view server)
{
  std::string uploadedFile = net->upload(file.data(), server.data());
  std::string url = "https://api.vk.com/method/";

  if (type == "photo") url += "photos.saveMessagesPhoto";

  simdjson::dom::object uploadObject = parser.parse(uploadedFile);

  std::string uploadServer =
    net->request(url + "?",
     {{ "photo",         std::string{uploadObject["photo"].get_c_str()}     },
      { "hash",          std::string{uploadObject["hash"].get_c_str()}      },
      { "server",        std::to_string(uploadObject["server"].get_int64()) },
      { "v",             apiVersion_       },
      { "access_token",  accessToken_      }
     });

  simdjson::dom::object uploadedAttachmentObject = parser.parse(uploadServer);

  return { uploadedAttachmentObject["response"].at(0)["owner_id"].get_int64(),
           uploadedAttachmentObject["response"].at(0)["id"].get_int64() };
}

std::string bot::VkAPI::processAttachmentUploading(std::string_view type, std::string_view file, std::string_view server, long peer_id)
{
  if (net->download(file.data(), server.data()) != 0) return "Ошибка при скачивании файла.";

  std::string uploadServer =
    net->request(appendVkUrl_("photos.getMessagesUploadServer"),
     {{ "access_token", accessToken_ },
      { "v",            apiVersion_  },
      { "peer_id",      std::to_string(peer_id) }
     });

  simdjson::dom::object uploadServerObject = parser.parse(uploadServer);
  std::pair<long, long> attachment = uploadAttachment_(type, file, std::string{uploadServerObject["response"]["upload_url"].get_c_str()});

  sendMessage("", peer_id, {{"attachment", "photo" + std::to_string(attachment.first) + "_" + std::to_string(attachment.second)}});
  return "";
}
