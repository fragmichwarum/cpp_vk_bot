#pragma once

#include <vector>

namespace simdjson::dom
{
class object;
}

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
  std::vector<long> conversations;
  /*!
   * @brief Database object.
   */
  static class Repository* repository;
  /*!
   * @brief VK API instance.
   */
  static class VkAPI* api;
  /*!
   * @brief Get conversations from Database.
   */
  void updateConversations_();

public:
  /*!
   * @brief Sends messages to chats about new post event.
   * @param update    - VK API response with new post event.
   */
//  void postMailing(const simdjson::dom::object& update);
  void postMailing(long from_id, long id);
 ~PostEventHandler();
};
}
