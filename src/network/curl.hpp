#ifndef CURL_HPP
#define CURL_HPP

#include <curl/curl.h>
#include "../cmd_holder/commands/lib/json.hpp"
#include "../long_poll/token.hpp"

using std::string;
using std::map;

class Curl {
private:
  static string urlencode(string& url);
  static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);

public:
  Curl& operator=(Curl&) = delete;
  static string generate_vk_query(string server, const map<string, string>& query);
  static string generate_query(string server, const map<string, string>& query);
  static string request(string url);
};

#endif // CURL_HPP
