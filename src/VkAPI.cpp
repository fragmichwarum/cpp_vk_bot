#include "VKAPI.hpp"

bot::VkAPI* bot::VkAPI::instance_ = nullptr;

bot::VkAPI::VkAPI()
  : net         (bot::Network::getInstance())
  , element     (parser.load("./init.json"))
  , accessToken (element["token"]["access_token"].get_c_str())
  , userToken   (element["token"]["user_token"].get_c_str())
  , groupId     (element["group_id"].get_c_str())
  , apiVersion  (element["api_v"].get_c_str())
  , logPath     (element["path"]["log"].get_c_str())
  , errPath     (element["path"]["err"].get_c_str())
{ }

bot::VkAPI* bot::VkAPI::getInstance()
{
  if (instance_ == nullptr) instance_ = new VkAPI;
  return instance_;
}

bot::VkAPI::LongPollData bot::VkAPI::getLongPollServer()
{
  std::string response =
    net->request(net->appendVkUrl("groups.getLongPollServer"),
     {{ "group_id",     groupId      },
      { "random_id",    "0"          },
      { "access_token", accessToken  },
      { "v",            apiVersion   }});

  simdjson::padded_string padded_string = response;

  return {
    std::string{parser.parse(padded_string)["response"]["key"].get_c_str()},
    std::string{parser.parse(padded_string)["response"]["server"].get_c_str()},
    std::string{parser.parse(padded_string)["response"]["ts"].get_c_str()}
  };
}

std::string bot::VkAPI::listenLongPoll(const std::string& key, const std::string& server, const std::string& ts)
{
  return net->request(server + '?', {{"act", "a_check"}, {"key", key}, {"ts", ts}, {"wait", "90"}});
}

void bot::VkAPI::sendMessage(const std::string& text, const long& peerId, const bot::traits::dictionary& options)
{
  traits::dictionary parameters = {
    { "message",      text         },
    { "peer_id",      std::to_string(peerId) },
    { "random_id",    "0"          },
    { "access_token", accessToken  },
    { "v",            apiVersion   },
    { "disable_mentions", "1"      }
  };

  if (options.size() != 0) {
    parameters.insert(options.begin(), options.end());
  }

  net->request(net->appendVkUrl("messages.send"), parameters);
}

std::string bot::VkAPI::getAttachmentType(const std::string& method)
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

std::string bot::VkAPI::searchMedia(const std::string& method, const std::string& keyword)
{
  std::string response =
      net->request(net->appendVkUrl(method),
       {{ "q",            keyword    },
        { "access_token", userToken  },
        { "v",            apiVersion },
        { "count",        "50"       }});

  simdjson::padded_string padded_string = response;
  simdjson::dom::parser parser;
  simdjson::dom::array items = parser.parse(padded_string)["response"]["items"].get_array();

  if (items.size() == 0) {
    return "";
  }

  std::string docs;
  for (uint8_t i = 0; i < items.size() && i < 10; i++) {
    long index = rand() % items.size();
    docs +=
      getAttachmentType(method) +
      std::to_string(items.at(index)["owner_id"].get_int64()) + '_' +
      std::to_string(items.at(index)["id"].get_int64()) + ',';
  }

  return docs;
}

std::string bot::VkAPI::kick(const long& chatId, const long& userId)
{
  std::string response =
    net->request(net->appendVkUrl("messages.removeChatUser"),
     {{ "chat_id",      std::to_string(chatId) },
      { "user_id",      std::to_string(userId) },
      { "random_id",    "0"         },
      { "access_token", accessToken },
      { "v",            apiVersion  }});

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

std::string bot::VkAPI::getConversationMembers(const long& peerId)
{
  return
    net->request(net->appendVkUrl("messages.getConversationMembers"),
     {{ "fields",       "online"     },
      { "peer_id",      std::to_string(peerId)},
      { "random_id",    "0"          },
      { "access_token", accessToken  },
      { "v",            apiVersion   }});
}

std::pair<long, long> bot::VkAPI::uploadAttachment(const std::string& type, const std::string& file, const std::string& server)
{
  std::string uploadedFile = net->upload(file, server);
  std::string url = "https://api.vk.com/method/";

  if (type == "photo") url += "photos.saveMessagesPhoto";

  simdjson::dom::parser parser;
  simdjson::dom::object uploadObject = parser.parse(uploadedFile);

  std::string uploadServer =
    net->request(url + "?",
     {{ "photo",         std::string{uploadObject["photo"].get_c_str()} },
      { "hash",          std::string{uploadObject["hash"].get_c_str()}  },
      { "server",        std::to_string(uploadObject["server"].get_int64()) },
      { "v",             apiVersion  },
      { "access_token",  accessToken }
     });

  simdjson::dom::object uploadedAttachmentObject = parser.parse(uploadServer);

  return { uploadedAttachmentObject["response"].at(0)["owner_id"].get_int64(),
           uploadedAttachmentObject["response"].at(0)["id"].get_int64() };
}

std::string bot::VkAPI::processAttachmentUploading(const std::string& type, const std::string& file, const std::string& server, const long& peer_id)
{
  std::mutex mutex;
mutex.lock();
  if (net->download(file, server) != 0) return "Ошибка при скачивании файла.";

  std::string uploadServer =
    net->request(net->appendVkUrl("photos.getMessagesUploadServer"),
     {{ "access_token", accessToken },
      { "peer_id",      std::to_string(peer_id) },
      { "album_id",     "0"         },
      { "group_id",     groupId     },
      { "v",            apiVersion  }
     });

  simdjson::dom::object uploadServerObject = parser.parse(uploadServer);

  std::pair<long, long> attachment = uploadAttachment(type, file, std::string{uploadServerObject["response"]["upload_url"].get_c_str()});
mutex.unlock();

  sendMessage("", peer_id, {{"attachment", "photo" + std::to_string(attachment.first) + "_" + std::to_string(attachment.second)}});
  return "";
}
