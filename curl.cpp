#include "curl.hpp"

std::string cURL::urlencode(std::string url) {
    std::string urlen;
        for (char c : url) {
            if (c == ' ') {
                urlen += "%20";
        } else if (c == '\n') {
                urlen += "%0A";
        } else {
            urlen += c;
        }
    }
    return urlen;
}

size_t cURL::write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size* nmemb);
    return size * nmemb;
}

std::string cURL::request(std::string url) {
    CURL* curl;
    CURLcode res;
    std::string buffer;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, cURL::urlencode(url).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
    }
    return buffer;
}

std::string cURL::generate_query(std::string server, query_vector &query) {
    std::string result = server;
    result = result + "access_token" + "=" + access_token + "&";
    result = result + "v" + "=" + api_version + "&";
    for (auto element : query) {
        result = result + element.first + "=" + element.second + "&";
    }
    std::cout << result << std::endl;
    return result;
}
