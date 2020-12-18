#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include "Repository.hpp"

namespace bot
{
class SQLiteRepository final : public Repository
{
private:
  const std::string name_;
  SQLite::Database database_;

  void open_();

public:
  SQLiteRepository(const std::string& tableName);
  void insertRole(long user_id, long peer_id, const std::string& role) override;
  std::vector<long> getRoles(long peer_id, const std::string& role = "") override;
  std::vector<long> getAllConversations() override;
  std::string getRole(long user_id, long peer_id) override;
 ~SQLiteRepository() = default;
};
}
