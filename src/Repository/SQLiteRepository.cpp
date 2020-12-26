#include "Repository/SQLiteRepository.hpp"


bot::SQLiteRepository::SQLiteRepository(std::string_view name_)
  : name(name_)
  , database(name + ".db3", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE)
{
  open_();
}

void bot::SQLiteRepository::open_()
{
  database.exec(
    "CREATE TABLE IF NOT EXISTS " + name + " ("
    "USER_ID INTEGER NOT NULL, "
    "PEER_ID INTEGER NOT NULL, "
    "ROLE TEXT NOT NULL);"
  );
}

void bot::SQLiteRepository::insertRole(long user_id, long peer_id, std::string_view role)
{
  database.exec(
    "DELETE FROM " + name + " WHERE USER_ID = " + std::to_string(user_id) +
    " AND PEER_ID = " + std::to_string(peer_id) + ";"
  );
  database.exec(
    "INSERT INTO " + name + " ('USER_ID','PEER_ID','ROLE')"
    "VALUES ('" + std::to_string(user_id) + "','" + std::to_string(peer_id) + "','" + role.data() + "');"
  );
}

std::vector<long> bot::SQLiteRepository::getRoles(long peer_id, std::string_view role)
{
  std::string query =
    role == "" ?
      "SELECT USER_ID FROM " + name + " WHERE PEER_ID = " + std::to_string(peer_id) + "  GROUP BY USER_ID;"
    : "SELECT USER_ID FROM " + name + " WHERE ROLE = '" + role.data() + "' AND PEER_ID = '" + std::to_string(peer_id) + "';";

  std::vector<long> userIds;
  SQLite::Statement statement(database, query);
  while (statement.executeStep()) {
    userIds.push_back(std::move(statement.getColumn(0)));
  }
  return userIds;
}

std::string bot::SQLiteRepository::getRole(long user_id, long peer_id)
{
  try {
    return database.execAndGet(
             "SELECT ROLE FROM " + name + " WHERE PEER_ID = " + std::to_string(peer_id) + " AND USER_ID = " + std::to_string(user_id) + ";"
           );
  } catch(SQLite::Exception&) {
    return "";
  }
}

std::vector<long> bot::SQLiteRepository::getAllConversations()
{
  std::string query = "SELECT PEER_ID FROM " + name + " GROUP BY PEER_ID";
  std::vector<long> peerIds;
  SQLite::Statement statement(database, query);

  while (statement.executeStep()) {
    peerIds.push_back(std::move(statement.getColumn(0)));
  }
  return peerIds;
}
