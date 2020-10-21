#ifndef CURL_HPP
#define CURL_HPP

#include <curl/curl.h>
#include "../cmd_holder/commands/lib/json.hpp"
#include "../long_poll/token.hpp"

using std::string;
using std::map;
using nlohmann::json;
using params = map<string, string>;

void   append_vkparams(params& map);
string append_vkurl   (const string& method);
json   http_processing(const string&, const params&);

class Curl {
private:
  static string urlencode(string& url);
  static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);

public:
  static string curl_gen    (const string& server, const params& query);
  static string request     (string url);
  static string send_request(const string& method, const params& params);
};

#endif // CURL_HPP
