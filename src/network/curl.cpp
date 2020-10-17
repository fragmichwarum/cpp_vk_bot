#include "curl.hpp"

string Curl::urlencode(string& url) { //костыль
  string urlen;
  for (char c : url) {
    if (c == ' ') {
      urlen += "%20";
    } else if (c == '\n') {
      urlen += "%0a";
    } else if (c == '+') {
      urlen += "%2b";
    } else {
      urlen += c;
    }
  }
  return urlen;
}

size_t Curl::write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
  ((string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

string Curl::request(string url) {
  string buffer;
  CURL* curl;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, Curl::urlencode(url).c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600L);
    curl_easy_perform(curl);
  }
  curl_easy_reset(curl);
 #ifdef LOG
  std::cout << url << std::endl;
 #endif
  return buffer;
}

string Curl::generate_vk_query(string method, const map<string, string>& query) {
  string result = api_url;
  result += method + "?";
  result += "access_token=" + access_token + "&";
  result += "v=" + api_version + "&";
  for (const auto& element : query) {
    result += element.first + "=" + element.second + "&";
  }
  return result;
}

string Curl::generate_query(string url, const map<string, string>& query) {
  string result = url;
  for (const auto& element : query) {
    result += element.first + "=" + element.second + "&";
  }
  return result;
}
