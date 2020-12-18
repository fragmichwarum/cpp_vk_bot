#pragma once

#include <vector>
#include <string>

namespace bot
{
class Repository
{
public:
  virtual void insertRole(long user_id, long peer_id, const std::string& role) = 0;
  virtual std::vector<long> getRoles(long peer_id, const std::string& role = "") = 0;
  virtual std::vector<long> getAllConversations() = 0;
  virtual std::string getRole(long user_id, long peer_id) = 0;
  virtual ~Repository() = default;
};
} //namespace bot
