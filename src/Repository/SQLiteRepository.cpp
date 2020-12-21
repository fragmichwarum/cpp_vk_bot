#include "Repository/SQLiteRepository.hpp"

bot::SQLiteRepository::SQLiteRepository(std::string_view name)
  : name_(name)
  , database_(name_ + ".db3", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE)
{
  open_();
}

void bot::SQLiteRepository::open_()
{
  database_.exec(
    "CREATE TABLE IF NOT EXISTS " + name_ + " ("
    "USER_ID INTEGER NOT NULL, "
    "PEER_ID INTEGER NOT NULL, "
    "ROLE TEXT NOT NULL);"
  );
}

void bot::SQLiteRepository::insertRole(long user_id, long peer_id, std::string_view role)
{
  database_.exec(
    "DELETE FROM " + name_ + " WHERE USER_ID = " + std::to_string(user_id) +
    " AND PEER_ID = " + std::to_string(peer_id) + ";"
  );
  database_.exec(
    "INSERT INTO " + name_ + " ('USER_ID','PEER_ID','ROLE')"
    "VALUES ('" + std::to_string(user_id) + "','" + std::to_string(peer_id) + "','" + role.data() + "');"
  );
}

std::vector<long> bot::SQLiteRepository::getRoles(long peer_id, std::string_view role)
{
  std::string query =
    role == "" ?
      "SELECT USER_ID FROM " + name_ + " WHERE PEER_ID = " + std::to_string(peer_id) + "  GROUP BY USER_ID;"
    : "SELECT USER_ID FROM " + name_ + " WHERE ROLE = '" + role.data() + "' AND PEER_ID = '" + std::to_string(peer_id) + "';";

  std::vector<long> userIds;
  SQLite::Statement statement(database_, query);
  while (statement.executeStep()) {
    userIds.push_back(std::move(statement.getColumn(0)));
  }
  return userIds;
}

std::string bot::SQLiteRepository::getRole(long user_id, long peer_id)
{
  try {
    return database_.execAndGet(
             "SELECT ROLE FROM " + name_ + " WHERE PEER_ID = " + std::to_string(peer_id) + " AND USER_ID = " + std::to_string(user_id) + ";"
           );
  } catch(SQLite::Exception&) {
    return "";
  }
}

std::vector<long> bot::SQLiteRepository::getAllConversations()
{
  std::string query = "SELECT PEER_ID FROM " + name_ + " GROUP BY PEER_ID";
  std::vector<long> peerIds;
  SQLite::Statement statement(database_, query);

  while (statement.executeStep()) {
    peerIds.push_back(std::move(statement.getColumn(0)));
  }
  return peerIds;
}
