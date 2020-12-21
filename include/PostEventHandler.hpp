#pragma once

#include <simdjson.h>

namespace bot
{
/*!
 * @brief Class that sends notifications about new posts.
 */
class PostEventHandler
{
private:
  /*!
   * @brief list of conversations.
   */
  std::vector<long> conversations_;
  /*!
   * @brief Database object.
   */
  static class Repository* repository_;
  /*!
   * @brief VK API instance.
   */
  static class VkAPI* api_;
  /*!
   * @brief Get conversations from Database.
   */
  void updateConversations_();

public:
  /*!
   * @brief Sends messages to chats about new post event.
   * @param update    - VK API response with new post event.
   */
  void postMailing(const simdjson::dom::object& update);
 ~PostEventHandler();
};
}
