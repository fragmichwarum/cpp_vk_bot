#pragma once

#include "../cmd_holder/lib/json.hpp"
#include "../long_poll/metadata.hpp"

namespace cURL {
using std::string;
using std::unordered_map;
using params = unordered_map<string, string>;

void   append_vkparams(params& map);
string append_vkurl   (const string& method);
string genparams      (const params& body);
string request        (string method, const params& body);
}
