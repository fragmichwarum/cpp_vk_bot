#pragma once

#include <map>
#include "../long_poll/metadata.hpp"

inline const std::string api_version = "5.80";
inline const std::string api_url     = "https://api.vk.com/method/";

namespace cURL {
using std::string;
using std::map;
using params = map<string, string>;

void   append_vkparams(params& map);
string append_vkurl   (const string& method);
string genparams      (const params& body);
string request        (string method, const params& body);
}
