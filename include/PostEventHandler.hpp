#pragma once

#include <vector>
#include <memory>

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
public:
  /*!
   * @brief <em>repository</em> and <em>api</em> initialize-constructor.
   */
  explicit
  PostEventHandler();
  /*!
   * @brief Destructor marked as <em>default</em>.
   *
   * It must be defined in place, when <em>std::unique_ptr</em> pointed-to type is complete.
   */
 ~PostEventHandler();
  /*!
   * @brief Sends messages to chats about new post event.
   * @param update    - VK API response with new post event.
   */
  void postMailing(long from_id, long id);

private:
  /*!
   * @brief list of conversations.
   */
  std::vector<long> conversations;
  /*!
   * @brief Database object.
   */
  std::unique_ptr<class Repository> repository;
  /*!
   * @brief VK API instance.
   */
  std::unique_ptr<class VkAPI> api;
  /*!
   * @brief Get conversations from Database.
   */
  void updateConversations_();
};
}
