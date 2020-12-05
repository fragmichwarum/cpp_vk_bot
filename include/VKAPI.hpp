#pragma once

#include <simdjson.h>
#include <mutex>

#include "Utility.hpp"
#include "Network.hpp"

namespace bot
{
class VkAPI
{
private:
  static VkAPI* instance_;

  Network* net = Network::getInstance();
  simdjson::dom::parser parser;
  simdjson::dom::element element;

  const std::string accessToken;
  const std::string userToken;
  const std::string groupId;

  std::string getAttachmentType(const std::string& method);

  VkAPI();

public:
  /*!
   *  Спорное решение...
   */
  const std::string apiVersion;
  const std::string logPath;
  const std::string errPath;

  struct LongPollData
  {
    std::string key;
    std::string server;
    std::string ts;
  };

  void operator=(VkAPI&) = delete;

  static VkAPI* getInstance();
  LongPollData getLongPollServer();
  std::string listenLongPoll(const std::string& key, const std::string& server, const std::string& ts);
  void sendMessage(const std::string& text, const long& peerId, const traits::dictionary& options = {});
  std::string searchMedia(const std::string& method, const std::string& keyword);
  std::string kick(const long& chatId, const long& userId);
  std::string getConversationMembers(const long& peerId);
  std::pair<long, long> uploadAttachment(const std::string& type, const std::string& file, const std::string& server);
  std::string processAttachmentUploading(const std::string& type, const std::string& file, const std::string& server, const long& peer_id);
};
} //namespace bot
