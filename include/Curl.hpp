#pragma once

#include <curl/curl.h>
#include <map>
#include <string>

namespace bot
{
namespace cURL
{
std::string urlencode(const std::string& url);
std::string appendVkUrl(const std::string& method);
std::string toJson(const std::map<std::string, std::string>& body);
std::string request(const std::string& body, const std::map<std::string, std::string>& params);
std::size_t download(const std::string& filename, const std::string& outputfile);
std::string upload(const std::string& filename, const std::string& server);
std::string requestdata(std::string body, const std::string& data);
} //namespace cURL
} //namespace bot
