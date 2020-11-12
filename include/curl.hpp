#pragma once

#include <curl/curl.h>
#include <map>
#include "metadata.hpp"

inline const std::string api_version = "5.124";
inline const std::string api_url     = "https://api.vk.com/method/";

namespace bot
{
namespace cURL
{
using params = std::map<std::string, std::string>;

std::string urlencode    (const std::string& url);
void append_vkparams     (params& map);
std::string append_vkurl (const std::string& method);
std::string genparams    (const params& body);
std::string to_json      (const params& body);
std::string request      (const std::string& method, const params& body);
std::string requestdata  (std::string method, const std::string& data);
} //namespace cURL
} //namespace bot
