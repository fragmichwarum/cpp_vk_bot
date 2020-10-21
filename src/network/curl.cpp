#include "curl.hpp"

namespace {
  string char_to_hex(const char c) {
    std::stringstream stream;
    if (static_cast<int>(c) < 0x10) {
      stream << "0";
    }
    stream << std::hex << static_cast<int>(c);
    return "%" + stream.str();
  }
}

string Curl::urlencode(string& url) {
  string urlen;
  for (char c : url) {
    if (c == ' ' || c == '\n' || c == '+' || c == '\\')
    {
      urlen += char_to_hex(c);
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
  curl_easy_cleanup(curl);
  std::cout << buffer << std::endl;
  return buffer;
}

string Curl::curl_gen(const string& url, const params& params) {
  string result = url;
  for (const auto& element : params) {
    result += element.first + "=" + element.second + "&";
  }
  return result;
}

string Curl::send_request(const string& method, const params& params) {
  return Curl::request(Curl::curl_gen(method, params));
}

void append_vkparams(params& map) {
  map["access_token"] = access_token;
  map["v"] = api_version;
}

string append_vkurl(const string &method) {
  return api_url + method + "?";
}

json http_processing(const string& method, const params& params) {
  return json::parse(Curl::send_request(method, params));
}
