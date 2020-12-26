#pragma once

#include <map>
#include <string>

#define vk_always_inline inline __attribute__((always_inline))

namespace bot
{
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
public:
  explicit
  VkAPI(const std::string& path = "./init.json");
 ~VkAPI();
  /*!
   * @return @ref apiVersion_
   */
  vk_always_inline std::string getApiVersion() const noexcept { return apiVersion_; }
  /*!
   * @return @ref logPath_
   */
  vk_always_inline std::string getLogPath() const noexcept { return logPath_; }
  /*!
   * @return @ref errPath_
   */
  vk_always_inline std::string getErrPath() const noexcept { return errPath_; }
  /*!
   * @brief Set new chat name.
   * @param chat_id     - id of chat.
   * @param title       - new title of a chat.
   */
  void editChat(long chat_id, std::string_view title);
  /*!
   * @brief Search media (photo, video, documents)
   * @param method      - name of method - <em>video.search</em>, <em>photos.search</em> or <em>docs.search</em>.
   * @param keyword     - search query
   * @param count       - count of returned attachments
   * @return Raw JSON response.
   */
  std::string mediaSearch(std::string_view method, std::string_view keyword, long count);
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

private:
  /*!
   * @brief Vkontakte group token.
   *
   * To get it, please go to Settings -> API usage -> Create token.
   */
  /*const*/ std::string accessToken_;
  /*!
   * @brief Vkontakte user token.
   *
   * To get it, please go to https://vkhost.github.io/.
   */
  /*const*/ std::string userToken_;
  /*!
   * @brief Your group ID.
   */
  /*const*/ std::string groupId_;
  /*!
   * @brief Vkontakte API version.
   */
  /*const*/ std::string apiVersion_;
  /*!
   * @brief Path to log file.
   */
  /*const*/ std::string logPath_;
  /*!
   * @brief Path to error log file.
   */
  /*const*/ std::string errPath_;

  /*!
   * @brief Network handler.
   */
  static class Network* net;
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
};
} //namespace bot
