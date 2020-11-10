#pragma once

#include <curl/curl.h>
#include <map>
#include "../long_poll/metadata.hpp"

inline const std::string api_version = "5.124";
inline const std::string api_url     = "https://api.vk.com/method/";

namespace cURL {
using std::string;
using std::map;
using params = map<string, string>;

string urlencode      (const string& url);
void   append_vkparams(params& map);
string append_vkurl   (const string& method);
string genparams      (const params& body);
string to_json        (const params& body);
string request        (const string& method, const params& body);
string requestdata    (string method, const string& data);
}
