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
  static string curl_gen    (const string& server, const map<string, string>& query);
  static string request     (string url);
  static string send_request(const string& method, const map<string, string>& params);
};

#endif // CURL_HPP
