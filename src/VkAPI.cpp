#include "Network.hpp"
#include "VkAPI.hpp"
#include "VKError.hpp"

bot::Network* bot::VkAPI::net_ = new bot::Network;
simdjson::dom::parser parser;

bot::VkAPI::VkAPI()
  : data_("./init.json")
  , accessToken_(data_.loadAccessToken())
  , userToken_  (data_.loadUserToken())
  , groupId_    (data_.loadGroupId())
  , apiVersion_ (data_.loadApiVersion())
  , logPath_    (data_.loadLogPath())
  , errPath_    (data_.loadErrorPath())
{ }

bot::VkAPI::~VkAPI()
{
  delete net_;
}

std::string bot::VkAPI::getApiVersion()
{
  return apiVersion_;
}

std::string bot::VkAPI::getErrPath()
{
  return errPath_;
}

std::string bot::VkAPI::getLogPath()
{
  return logPath_;
}

std::string bot::VkAPI::appendVkUrl_(const std::string& method)
{
  return "https://api.vk.com/method/" + method + '?';
}

bot::LongPollData bot::VkAPI::getLongPollServer()
{
  std::string response =
    net_->request(appendVkUrl_("groups.getLongPollServer"),
     {{ "group_id",     groupId_      },
      { "random_id",    "0"           },
      { "access_token", accessToken_  },
      { "v",            apiVersion_   }});

  simdjson::dom::object parsed = parser.parse(response);

  if (parsed.begin().key() == "error") throw Vk_exception(parsed["error"]["error_code"].get_int64());

  return {
    std::string{parser.parse(response)["response"]["key"].get_c_str()},
    std::string{parser.parse(response)["response"]["server"].get_c_str()},
    std::string{parser.parse(response)["response"]["ts"].get_c_str()}
  };
}

std::string bot::VkAPI::listenLongPoll(const std::string& key, const std::string& server, const std::string& ts)
{
  return net_->request(server + '?', {{"act", "a_check"}, {"key", key}, {"ts", ts}, {"wait", "90"}});
}

void bot::VkAPI::sendMessage(const std::string& text, long peerId, const bot::dictionary& options)
{
  dictionary parameters = {
    { "message",      text         },
    { "peer_id",      std::to_string(peerId) },
    { "random_id",    "0"          },
    { "access_token", accessToken_ },
    { "v",            apiVersion_  },
    { "disable_mentions", "1"      }
  };

  if (options.size() != 0) parameters.insert(options.begin(), options.end());

  net_->request(appendVkUrl_("messages.send"), parameters);
}

std::string bot::VkAPI::getAttachmentType_(const std::string& method)
{
  if (method == "photos.search") return "photo";
  if (method == "video.search") return "video";
  if (method == "docs.search") return "doc";
  return "";
}

std::string bot::VkAPI::searchMedia(const std::string& method, const std::string& keyword)
{
  std::string response =
      net_->request(appendVkUrl_(method),
       {{ "q",            keyword     },
        { "access_token", userToken_  },
        { "v",            apiVersion_ },
        { "count",        "50"        }});

  simdjson::dom::array items = parser.parse(response)["response"]["items"].get_array();

  if (items.size() == 0) return "";

  std::string docs;
  for (uint8_t i = 0; i < items.size() && i < 10; i++) {
    long index = rand() % items.size();
    docs +=
      getAttachmentType_(method) +
      std::to_string(items.at(index)["owner_id"].get_int64()) + '_' +
      std::to_string(items.at(index)["id"].get_int64()) + ',';
  }

  return docs;
}

std::string bot::VkAPI::kick(long chatId, long userId)
{
  std::string response =
    net_->request(appendVkUrl_("messages.removeChatUser"),
     {{ "chat_id",      std::to_string(chatId) },
      { "user_id",      std::to_string(userId) },
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

std::string bot::VkAPI::getConversationMembers(long peerId)
{
  return
    net_->request(appendVkUrl_("messages.getConversationMembers"),
     {{ "fields",       "online"      },
      { "peer_id",      std::to_string(peerId) },
      { "random_id",    "0"           },
      { "access_token", accessToken_  },
      { "v",            apiVersion_   }});
}

void bot::VkAPI::editChat(long chat_id, const std::string& title)
{
  net_->request(appendVkUrl_("messages.editChat"),
   {{ "chat_id",      std::to_string(chat_id - 2000000000) },
    { "title",        title        },
    { "random_id",    "0"          },
    { "access_token", accessToken_ },
    { "v",            apiVersion_  }});
}

std::pair<long, long> bot::VkAPI::uploadAttachment_(const std::string& type, const std::string& file, const std::string& server)
{
  std::string uploadedFile = net_->upload(file, server);
  std::string url = "https://api.vk.com/method/";

  if (type == "photo") url += "photos.saveMessagesPhoto";

  simdjson::dom::object uploadObject = parser.parse(uploadedFile);

  std::string uploadServer =
    net_->request(url + "?",
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

std::string bot::VkAPI::processAttachmentUploading(const std::string& type, const std::string& file, const std::string& server, long peer_id)
{
  if (net_->download(file, server) != 0) return "Ошибка при скачивании файла.";

  std::string uploadServer =
    net_->request(appendVkUrl_("photos.getMessagesUploadServer"),
     {{ "access_token", accessToken_ },
      { "v",            apiVersion_  },
      { "peer_id",      std::to_string(peer_id) }
     });

  simdjson::dom::object uploadServerObject = parser.parse(uploadServer);
  std::pair<long, long> attachment = uploadAttachment_(type, file, std::string{uploadServerObject["response"]["upload_url"].get_c_str()});

  sendMessage("", peer_id, {{"attachment", "photo" + std::to_string(attachment.first) + "_" + std::to_string(attachment.second)}});
  return "";
}
