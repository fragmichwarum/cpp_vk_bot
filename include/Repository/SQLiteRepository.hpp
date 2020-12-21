#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include "Repository.hpp"

namespace bot
{
/*!
 * @brief The Database interface implementation.
 */
class SQLiteRepository final : public Repository
{
private:
  const std::string name_;
  SQLite::Database database_;

  void open_();

public:
  SQLiteRepository(std::string_view tableName);
  /*!
   * @brief Insert role to table.
   * @param user_id     - id of user.
   * @param peer_id     - chat id.
   * @param role        - name of role to set.
   */
  void insertRole(long user_id, long peer_id, std::string_view role) override;
  /*!
   * @param peer_id     - id of user.
   * @param role        - name of role.
   * @return User id's in current conversation with selected role.
   */
  std::vector<long> getRoles(long peer_id, std::string_view role = "") override;
  /*!
   * @return List of all previously saved conversations.
   */
  std::vector<long> getAllConversations() override;
  /*!
   * @brief Get role of selected user in selected chat.
   * @param user_id     - id of user.
   * @param peer_id     - chat id.
   * @return User role.
   */
  std::string getRole(long user_id, long peer_id) override;
 ~SQLiteRepository() = default;
};
}
