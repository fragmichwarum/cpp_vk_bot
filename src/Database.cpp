#include "Database.hpp"

bot::Database::Database()
  : database("users.db3", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE)
  , name("USERS")
{
  open();
}

bot::Database::Database(const std::string& name)
  : database("users.db3", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE)
  , name(name)
{
  open();
}

void bot::Database::open()
{
  database.exec(
    "CREATE TABLE IF NOT EXISTS " + name + " ("
    "USER_ID INTEGER NOT NULL, "
    "PEER_ID INTEGER NOT NULL, "
    "ROLE TEXT NOT NULL);"
  );
}

void bot::Database::insertRole(long user_id, long peer_id, const std::string& role)
{
  database.exec(
    "DELETE FROM USERS WHERE USER_ID = " + std::to_string(user_id) +
    " AND PEER_ID = " + std::to_string(peer_id) + ";"
  );
  database.exec(
    "INSERT INTO USERS('USER_ID','PEER_ID','ROLE') "
    "SELECT USER_ID,PEER_ID,ROLE "
    "FROM (SELECT '" + std::to_string(user_id) + "' AS USER_ID, "
    "'" + std::to_string(peer_id) + "' AS PEER_ID, "
    "'" + role + "' AS ROLE) T "
    "WHERE NOT EXISTS (SELECT 1 FROM USERS WHERE USERS.USER_ID = T.USER_ID AND USERS.PEER_ID = T.PEER_ID);"
  );
}

std::vector<long> bot::Database::getRoles(long peer_id, const std::string& role)
{
  std::string query =
    role == "" ?
      "SELECT USER_ID FROM USERS WHERE PEER_ID = " + std::to_string(peer_id) + "  GROUP BY USER_ID;"
    : "SELECT USER_ID FROM USERS WHERE ROLE = '" + role + "' AND PEER_ID = '" + std::to_string(peer_id) + "';";

  std::vector<long> userIds;
  SQLite::Statement statement(database, query);
  while (statement.executeStep()) {
    userIds.push_back(std::move(statement.getColumn(0)));
  }
  return userIds;
}

std::string bot::Database::getRole(long user_id, long peer_id)
{
  try {
    return database.execAndGet(
             "SELECT ROLE FROM USERS WHERE PEER_ID = " + std::to_string(peer_id) + " AND USER_ID = " + std::to_string(user_id) + ";"
           );
  } catch(SQLite::Exception&) {
    return "";
  }
}

std::vector<long> bot::Database::getAllConversations()
{
  std::string query = "SELECT PEER_ID FROM USERS GROUP BY PEER_ID";
  std::vector<long> peerIds;
  SQLite::Statement statement(database, query);

  while (statement.executeStep()) {
    peerIds.push_back(std::move(statement.getColumn(0)));
  }
  return peerIds;
}
