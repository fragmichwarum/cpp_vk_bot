#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>

namespace bot
{
class Database
{
private:
  SQLite::Database database;
  std::string      name;
  void open();

public:
  Database();
  Database(const std::string& name);
  void insertRole(long user_id, long peer_id, const std::string& role);
  std::vector<long> getRoles(long peer_id, const std::string& role = "");
  std::vector<long> getAllConversations();
  std::string getRole(long user_id, long peer_id);
};
} //namespace bot
