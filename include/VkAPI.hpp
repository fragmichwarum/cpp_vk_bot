#pragma once

#include <simdjson.h>
#include <map>

namespace bot
{
class _UserData
{
public:
  _UserData(const _UserData&)            = delete;
  _UserData& operator=(const _UserData&) = delete;
  _UserData& operator=(_UserData&&)      = delete;

private:
  friend class VkAPI;

  simdjson::dom::parser parser_;
  simdjson::dom::element element_;

  _UserData(const std::string& file) { element_ = parser_.load(file); }
  std::string loadAccessToken() { return std::string{ element_["token"]["access_token"].get_c_str() }; }
  std::string loadUserToken()   { return std::string{ element_["token"]["user_token"].get_c_str() }; }
  std::string loadApiVersion()  { return std::string{ element_["api_v"].get_c_str() }; }
  std::string loadGroupId()     { return std::string{ element_["group_id"].get_c_str() }; }
  std::string loadLogPath()     { return std::string{ element_["path"]["log"].get_c_str() }; }
  std::string loadErrorPath()   { return std::string{ element_["path"]["err"].get_c_str() }; }
};

struct LongPollData
{
  std::string key;
  std::string server;
  std::string ts;
};

class VkAPI
{
private:
  _UserData data_;
  const std::string accessToken_;
  const std::string userToken_;
  const std::string groupId_;
  const std::string apiVersion_;
  const std::string logPath_;
  const std::string errPath_;

  static class Network* net_;

  std::string getAttachmentType_(const std::string& method);
  std::string appendVkUrl_(const std::string& method);
  std::pair<long, long> uploadAttachment_(const std::string& type, const std::string& file, const std::string& server);

public:
  VkAPI();
 ~VkAPI();

  std::string getApiVersion();
  std::string getLogPath();
  std::string getErrPath();

  void        editChat(long chat_id, const std::string& title);
  void        sendMessage(const std::string& text, long peer_id, const std::map<std::string, std::string>& options = {});
  LongPollData getLongPollServer();
  std::string listenLongPoll(const std::string& key, const std::string& server, const std::string& ts);
  std::string searchMedia(const std::string& method, const std::string& keyword);
  std::string kick(long chat_id, long user_id);
  std::string getConversationMembers(long peer_id);
  std::string processAttachmentUploading(const std::string& type, const std::string& file, const std::string& server, long peer_id);
};
} //namespace bot
