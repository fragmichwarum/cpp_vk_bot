#pragma once

#include <curl/curl.h>
#include <map>
#include "metadata.hpp"

namespace bot
{
inline const std::string api_version = "5.124";
inline const std::string api_url     = "https://api.vk.com/method/";

namespace cURL
{
std::string urlencode    (const std::string& url);
void append_vkparams     (std::map<std::string, std::string>& map);
std::string append_vkurl (const std::string& method);
std::string genparams    (const std::map<std::string, std::string>& body);
std::string to_json      (const std::map<std::string, std::string>& body);
std::string request      (const std::string& method, const std::map<std::string, std::string>& body);
std::string requestdata  (std::string method, const std::string& data);
} //namespace cURL
} //namespace bot
