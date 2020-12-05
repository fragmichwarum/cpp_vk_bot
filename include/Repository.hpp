#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>

namespace bot
{
class Repository
{
private:
  SQLite::Database database;
  std::string name;
  void open();

  static Repository* repository_;
  Repository();

public:
  Repository(Repository&) = delete;
  void operator=(const Repository&) = delete;

  static Repository* getInstance();
  void insertRole(long user_id, long peer_id, const std::string& role);
  std::vector<long> getRoles(long peer_id, const std::string& role = "");
  std::vector<long> getAllConversations();
  std::string getRole(long user_id, long peer_id);
};
} //namespace bot
