#include "Repository.hpp"

bot::Repository* bot::Repository::repository_ = nullptr;

bot::Repository::Repository()
  : database("users.db3", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE)
  , name("USERS")
{
  open();
  printf("repository default constructor\n");
}

bot::Repository* bot::Repository::getInstance()
{
  if (repository_ == nullptr) {
    repository_ = new Repository();
  }
  return repository_;
}

void bot::Repository::open()
{
  database.exec(
    "CREATE TABLE IF NOT EXISTS " + name + " ("
    "USER_ID INTEGER NOT NULL, "
    "PEER_ID INTEGER NOT NULL, "
    "ROLE TEXT NOT NULL);"
  );
}

void bot::Repository::insertRole(long user_id, long peer_id, const std::string& role)
{
  database.exec(
    "DELETE FROM " + name + " WHERE USER_ID = " + std::to_string(user_id) +
    " AND PEER_ID = " + std::to_string(peer_id) + ";"
  );
  database.exec(
    "INSERT INTO " + name + " ('USER_ID','PEER_ID','ROLE')"
    "VALUES ('" + std::to_string(user_id) + "','" + std::to_string(peer_id) + "','" + role + "');"
  );
}

std::vector<long> bot::Repository::getRoles(long peer_id, const std::string& role)
{
  std::string query =
    role == "" ?
      "SELECT USER_ID FROM " + name + " WHERE PEER_ID = " + std::to_string(peer_id) + "  GROUP BY USER_ID;"
    : "SELECT USER_ID FROM " + name + " WHERE ROLE = '" + role + "' AND PEER_ID = '" + std::to_string(peer_id) + "';";

  std::vector<long> userIds;
  SQLite::Statement statement(database, query);
  while (statement.executeStep()) {
    userIds.push_back(std::move(statement.getColumn(0)));
  }
  return userIds;
}

std::string bot::Repository::getRole(long user_id, long peer_id)
{
  try {
    return database.execAndGet(
             "SELECT ROLE FROM " + name + " WHERE PEER_ID = " + std::to_string(peer_id) + " AND USER_ID = " + std::to_string(user_id) + ";"
           );
  } catch(SQLite::Exception&) {
    return "";
  }
}

std::vector<long> bot::Repository::getAllConversations()
{
  std::string query = "SELECT PEER_ID FROM " + name + " GROUP BY PEER_ID";
  std::vector<long> peerIds;
  SQLite::Statement statement(database, query);

  while (statement.executeStep()) {
    peerIds.push_back(std::move(statement.getColumn(0)));
  }
  return peerIds;
}
