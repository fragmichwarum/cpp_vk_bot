#ifndef VK_HPP
#define VK_HPP

#include <string>
#include <vector>
#include <curl/curl.h>
#include <json.hpp>
#include "src/Lightning/Hash/crc32.hpp"
#include "src/Lightning/Hash/sha256.hpp"
#include "token.hpp"

using lightning::CRC32;
using lightning::SHA256;
using query_vector = std::vector<std::pair<std::string, std::string>>;

class cURL {
public:
    static std::string generate_query(std::string server, query_vector& query);
    static std::string urlencode(std::string url);
    static std::string request(std::string url);
private:
    static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif // VK_HPP
