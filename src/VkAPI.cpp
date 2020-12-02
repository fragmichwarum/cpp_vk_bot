#include <fstream>
#include <mutex>

#include "Curl.hpp"
#include "Utils.hpp"
#include "VkAPI.hpp"
#include "Info.hpp"

simdjson::dom::parser parser;
simdjson::dom::element element = parser.load("./init.json");

long bot::info::processedMessages(0);
const long bot::info::adminId           (element["admin_id"].get_int64());
const std::string bot::info::groupId    (element["group_id"].get_c_str());
const std::string bot::info::accessToken(element["token"]["access_token"].get_c_str());
const std::string bot::info::userToken  (element["token"]["user_token"].get_c_str());
const std::string bot::info::version    (element["api_v"].get_c_str());
const std::string bot::info::errfile    (element["path"]["err"].get_c_str());
const std::string bot::info::logfile    (element["path"]["log"].get_c_str());

void bot::api::sendMessage(const std::string& text, const long& peer_id, const traits::dictionary& options)
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

std::string bot::api::mediaSearch(const std::string& method, const std::string& text)
{
  std::string response =
      cURL::request(cURL::appendVkUrl(method),
       {{ "q",            text             },
        { "access_token", info::userToken  },
        { "v",            info::version    },
        { "count",        "50"             }});

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
      attachment_type(method) +
      std::to_string(items.at(index)["owner_id"].get_int64()) + '_' +
      std::to_string(items.at(index)["id"].get_int64()) + ',';
  }

  return docs;
}

std::string bot::api::kickUser(const long& chat_id, const long& user_id)
{

  std::string response =
    cURL::request(cURL::appendVkUrl("messages.removeChatUser"),
     {{ "chat_id",      std::to_string(chat_id) },
      { "user_id",      std::to_string(user_id) },
      { "random_id",    "0"                },
      { "access_token", info::accessToken  },
      { "v",            info::version      }});

  simdjson::padded_string padded_string = response;
  simdjson::dom::parser parser;
  simdjson::dom::object parsed = parser.parse(padded_string);

  if (not parsed["error"].is_null() &&
          parsed["error"]["error_code"].get_int64() == 100)
  {
    return "Что-то пошло не так.";
  }

  if (not parsed["error"].is_null() &&
          parsed["error"]["error_code"].get_int64() == 15)
  {
    return "Не могу кикнуть этого юзера/группу.";
  }

  return "Arbeit macht frei.";
}

std::pair<long, long> bot::api::uploadAttachment(const std::string& type, const std::string& file, const std::string& server)
{
  std::string uploadedFile = cURL::upload(file, server);
  std::string url = "https://api.vk.com/method/";

  if (type == "photo") {
    url += "photos.saveMessagesPhoto";
  }

  simdjson::dom::parser parser;
  simdjson::dom::object uploadObject = parser.parse(uploadedFile);

  std::string uploadServer =
    cURL::request(url + "?",
     {{ "photo",         std::string{uploadObject["photo"].get_c_str()} },
      { "hash",          std::string{uploadObject["hash"].get_c_str()}  },
      { "server",        std::to_string(uploadObject["server"].get_int64()) },
      { "v",             info::version          },
      { "access_token",  info::accessToken      }
     });

  simdjson::dom::object uploadedAttachmentObject = parser.parse(uploadServer);

  return { uploadedAttachmentObject["response"].at(0)["owner_id"].get_int64(),
           uploadedAttachmentObject["response"].at(0)["id"].get_int64() };
}

std::string bot::api::processAttachmentUploading(const std::string& type, const std::string& file, const std::string& server, const long& peer_id)
{
  std::mutex mutex;
mutex.lock();
  if (cURL::download(file, server) != 0) {
    return "Ошибка при скачивании файла.";
  }

  std::string uploadServer =
    cURL::request(cURL::appendVkUrl("photos.getMessagesUploadServer"),
     {{ "access_token", info::accessToken  },
      { "peer_id",      std::to_string(peer_id) },
      { "album_id",     "0"                },
      { "group_id",     info::groupId      },
      { "v",            info::version      }
     });

  simdjson::dom::object uploadServerObject = parser.parse(uploadServer);

  std::pair<long, long> attachment = api::uploadAttachment(type, file, std::string{uploadServerObject["response"]["upload_url"].get_c_str()});
mutex.unlock();

  api::sendMessage("", peer_id, {{"attachment", "photo" + std::to_string(attachment.first)
                                   + "_" + std::to_string(attachment.second)}});
  return "";
}
