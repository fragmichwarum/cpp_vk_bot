#pragma once

#include <vector>
#include <string>

namespace bot
{
/*!
 * @brief The Database interface.
 */
class Repository
{
public:
  /*!
   * @brief Insert role to table.
   * @param user_id     - id of user.
   * @param peer_id     - chat id.
   * @param role        - name of role to set.
   */
  virtual void insertRole(long user_id, long peer_id, std::string_view role) = 0;
  /*!
   * @param peer_id     - id of user.
   * @param role        - name of role.
   * @return User id's in current conversation with selected role.
   */
  virtual std::vector<long> getRoles(long peer_id, std::string_view role = "") = 0;
  /*!
   * @return List of all previously saved conversations.
   */
  virtual std::vector<long> getAllConversations() = 0;
  /*!
   * @brief Get role of selected user in selected chat.
   * @param user_id     - id of user.
   * @param peer_id     - chat id.
   * @return User role.
   */
  virtual std::string getRole(long user_id, long peer_id) = 0;
  virtual ~Repository() = default;
};
} //namespace bot
