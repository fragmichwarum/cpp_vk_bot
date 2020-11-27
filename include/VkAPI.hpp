#pragma once

#include "../lib/include/Json.hpp"
#include "Traits.hpp"

namespace bot
{
namespace vkapi
{
void send_message(const std::string& text, const long& peer_id, const traits::dictionary& options = {});
std::string kick_user(const long& chat_id, const long& user_id);
std::string media_search(const std::string& method, const std::string& text);
std::pair<long, long> upload_attachment(const std::string& type, const std::string& file, const std::string& server);
}
}
