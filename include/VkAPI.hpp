#pragma once

#include <simdjson.h>
#include <map>

#define vk_always_inline inline __attribute__((always_inline))

namespace bot
{
/*!
 * @brief The @ref bot::VkAPI private fields initializer.
 */
class _UserData
{
public:
  _UserData(const _UserData&)            = delete;
  _UserData& operator=(const _UserData&) = delete;
  _UserData& operator=(_UserData&&)      = delete;
  friend class VkAPI;

private:
  simdjson::dom::parser parser_;
  simdjson::dom::element element_;

  _UserData(std::string_view file) { element_ = parser_.load(file.data()); }
  vk_always_inline std::string_view loadAccessToken() const noexcept { return static_cast<const char*>(element_["token"]["access_token"].get_c_str()); }
  vk_always_inline std::string_view loadUserToken()   const noexcept { return static_cast<const char*>(element_["token"]["user_token"].get_c_str()); }
  vk_always_inline std::string_view loadApiVersion()  const noexcept { return static_cast<const char*>(element_["api_v"].get_c_str()); }
  vk_always_inline std::string_view loadGroupId()     const noexcept { return static_cast<const char*>(element_["group_id"].get_c_str()); }
  vk_always_inline std::string_view loadLogPath()     const noexcept { return static_cast<const char*>(element_["path"]["log"].get_c_str()); }
  vk_always_inline std::string_view loadErrorPath()   const noexcept { return static_cast<const char*>(element_["path"]["err"].get_c_str()); }
};

/*!
 * @brief Data type needed by @ref bot::VkAPI::getLongPollServer.
 */
struct LongPollData
{
  std::string key;
  std::string server;
  std::string ts;
};

/*!
 * @brief The Vkontakte API handler.
 *
 * friend of @ref _UserData class.
 */
class VkAPI
{
private:
  /*!
   * @brief Internal data initializer.
   */
  _UserData data;
  /*!
   * @brief Vkontakte group token.
   *
   * To get it, please go to Settings -> API usage -> Create token.
   */
  const std::string accessToken_;
  /*!
   * @brief Vkontakte user token.
   *
   * To get it, please go to https://vkhost.github.io/.
   */
  const std::string userToken_;
  /*!
   * @brief Your group ID.
   */
  const std::string groupId_;
  /*!
   * @brief Vkontakte API version.
   */
  const std::string apiVersion_;
  /*!
   * @brief Path to log file.
   */
  const std::string logPath_;
  /*!
   * @brief Path to error log file.
   */
  const std::string errPath_;

  /*!
   * @brief Network handler.
   */
  static class Network* net;

  /*!
   * @param VK API media search method name.
   * @return Attachment type.
   */
  std::string getAttachmentType_(std::string_view method) noexcept;
  /*!
   * @param Method name.
   * @return Ready to use URL.
   */
  std::string appendVkUrl_(std::string_view method);
  /*!
   * @brief Upload file to VK upload server.
   * @param Type of attachment.
   * @param File name.
   * @param Server address.
   * @return Owner id and id of just loaded attachment.
   */
  std::pair<long, long> uploadAttachment_(std::string_view type, std::string_view file, std::string_view server);

public:
  VkAPI();
 ~VkAPI();

  /*!
   * @return @ref apiVersion_
   */
  vk_always_inline std::string getApiVersion() { return apiVersion_; }
  /*!
   * @return @ref logPath_
   */
  vk_always_inline std::string getLogPath() { return logPath_; }
  /*!
   * @return @ref errPath_
   */
  vk_always_inline std::string getErrPath() { return errPath_; }
  /*!
   * @brief Set new chat name.
   * @param chat_id     - id of chat.
   * @param title       - new title of a chat.
   */
  void editChat(long chat_id, std::string_view title);
  /*!
   * @brief Send message.
   * @param text        - message content.
   * @param peer_id     - id of chat or private dialog.
   * @param options     - additional parameters like <em><b>attachment</b></em>.
   */
  void sendMessage(std::string_view text, long peer_id, const std::map<std::string, std::string>& options = {});
  /*!
   * @brief Get Long Poll response.
   * @return key, server, ts.
   */
  LongPollData getLongPollServer();
  /*!
   * @brief Start waiting for events from VK.
   * @param key         - default parameter returned by VK Long Poll.
   * @param server      - default parameter returned by VK Long Poll.
   * @param ts          - default parameter returned by VK Long Poll.
   * @return Long Poll response.
   */
  std::string listenLongPoll(std::string_view key, std::string_view server, std::string_view ts);
  /*!
   * @brief Get VK media.
   * @param method      - name of search method - <em><b>photos.search</b></em>, <em><b>video.search</b></em>, <em><b>docs.search</b></em>.
   * @param keyword     - search query.
   * @return List of attachments, empty string otherwise.
   */
  std::string searchMedia(std::string_view method, std::string_view keyword);
  /*!
   * @brief Kick user from chat.
   * @param chat_id
   * @param user_id
   * @return Farewell message :), error message otherwise.
   */
  std::string kick(long chat_id, long user_id);
  /*!
   * @brief Get information about chat members.
   * @param peer_id
   * @return The <em><b>messages.getConversationMembers</b></em> response.
   */
  std::string getConversationMembers(long peer_id);
  /*!
   * @brief Upload attachment and send message with it.
   * @param type        - type of VK attachment.
   * @param file        - filename.
   * @param server      - upload server address.
   * @param peer_id     - chat to send a message.
   * @return Empty string on success, error message otherwise.
   */
  std::string processAttachmentUploading(std::string_view type, std::string_view file, std::string_view server, long peer_id);
};
} //namespace bot
