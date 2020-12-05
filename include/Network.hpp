#pragma once

#include <curl/curl.h>

#include "Traits.hpp"

namespace bot
{
class Network
{
private:
  static std::string urlencode(const std::string& text);
  static size_t write(void* contents, size_t size, size_t nmemb, void* userp);
  static size_t file_write(void* ptr, size_t size, size_t nmemb, FILE* stream);
  std::string genparams(const bot::traits::dictionary& body) const;

  static Network* instance_;
  Network();

public:
  static Network* getInstance();
  Network(Network&) = delete;
  void operator=(Network&) = delete;

  std::string toJson(const traits::dictionary& body) const;
  std::string appendVkUrl(const std::string& method) const;
  std::string request(const std::string& method, const traits::dictionary& params) const;
  std::size_t download(const std::string& filename, const std::string& server) const;
  std::string upload(const std::string& filename, const std::string& server) const;
  std::string requestdata(const std::string& body, const std::string& data) const;
};
}
