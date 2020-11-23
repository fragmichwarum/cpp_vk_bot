#pragma once

#include "../lib/include/Json.hpp"

namespace bot
{
namespace info
{
extern const long        admin_id;
extern const std::string version;
extern const std::string access_token;
extern const std::string user_token;
extern const std::string group_id;
extern const std::string errfile;
extern const std::string logfile;
}

namespace vkapi
{
void send_message(const std::string& text, const long& peer_id, const std::map<std::string, std::string>& options = {});
std::string kick_user(const long& chat_id, const long& user_id);
std::string media_search(const std::string& method, const std::string& text);
std::pair<long, long> upload_attachment(const std::string& type, const std::string& file, const std::string& server);
}
}
