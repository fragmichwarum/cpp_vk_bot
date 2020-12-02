#pragma once

#include <simdjson.h>
#include "Traits.hpp"

namespace bot
{
namespace api
{
void sendMessage(const std::string& text, const long& peer_id, const traits::dictionary& options = {});
std::string kickUser(const long& chat_id, const long& user_id);
std::string mediaSearch(const std::string& method, const std::string& text);
std::pair<long, long> uploadAttachment(const std::string& type, const std::string& file, const std::string& server);
std::string processAttachmentUploading(const std::string& type, const std::string& file, const std::string& server, const long& peer_id);
}
}
