#pragma once

#include <curl/curl.h>
#include <map>

#include "Traits.hpp"

namespace bot
{
namespace cURL
{
const std::string urlencode(const std::string& url);
const std::string appendVkUrl(const std::string& method);
const std::string toJson(const traits::dictionary& body);
const std::string request(const std::string& body, const traits::dictionary& params);
std::size_t download(const std::string& filename, const std::string& outputfile);
const std::string upload(const std::string& filename, const std::string& server);
const std::string requestdata(const std::string& body, const std::string& data);
} //namespace cURL
} //namespace bot
