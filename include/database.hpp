#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

namespace bot
{
/*!
 * @brief Class that provides interface to work with SQLite3.
 */
class Database
{
private:
  /*!
   * @brief SQLite3 object.
   */
  sqlite3* database;
  /*!
   * @brief Error code.
   */
  int rc = 0;
  /*!
   * @brief String reserved for error message.
   */
  char* err = nullptr;
  /*!
   * @brief Boolean value needed to check if the DBMS is currently opened.
   */
  bool is_opened = false;

  /*!
   * @brief Callback function, needed by sqlite3_exec function.
   * @param not_used
   * @param argc
   * @param argv
   * @param col_name
   * @return 0.
   */
  static int callback(void* not_used, int argc, char* argv[], char** col_name);

public:
  /*!
   * @brief Opens the database if not opened.
   */
  void open();
  /*!
   * @brief Creates the database if not exists.
   */
  void init_table();
  /*!
   * @brief Inserts user role to the table.
   * @param Id of the user receiving the role
   * @param Peer id, where user gets role
   * @param Name of role
   */
  void insert_role(const long& user_id, const long& peer_id, const std::string& role);
  /*!
   * @brief Get user ids with all or specified role in the chat.
   * @param Peer id, where role must be getted
   * @param Role name
   * @param Vector of user_ids with selected(or not) role.
   */
  std::vector<long> get_roles(const long& peer_id, const std::string& role = "");
  /*!
   * @brief Get role of user in the chat.
   * @param User id
   * @param Peer id, where role must be getted
   * @param Setted role of user.
   */
  std::string get_role(const long& user_id, const long& peer_id);
  /*!
   * @brief Get all the peer ids visited by the bot.
   * @return Vector of peer_ids.
   */
  std::vector<long> get_peer_ids();
};
} //namespace bot
