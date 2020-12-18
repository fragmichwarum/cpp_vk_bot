#pragma once

#include <curl/curl.h>
#include <map>
#include <string>

namespace bot
{
using dictionary = std::map<std::string, std::string>;

class Network
{
private:
  CURL* curl_handle_ = curl_easy_init();

public:
 ~Network();
  std::string request(const std::string& method, const dictionary& params) const;
  std::size_t download(const std::string& filename, const std::string& server) const;
  std::string upload(const std::string& filename, const std::string& server) const;
  std::string requestdata(const std::string& body, const std::string& data) const;
};
}
