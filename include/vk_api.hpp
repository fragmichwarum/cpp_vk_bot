#pragma once

#include "lib/include/json.hpp"
#include "curl.hpp"
#include "utils.hpp"

namespace bot
{
class Vk_api
{
public:
  std::string kick_user(const long& chat_id, const long& user_id);
  void send_message(const std::string& text, const long& peer_id, const std::map<std::string, std::string>& options = {});
  std::string media_search(const std::string& method, const std::string& text);
};
} //namespace bot
