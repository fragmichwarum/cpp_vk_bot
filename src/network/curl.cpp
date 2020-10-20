#include "curl.hpp"

namespace {
  constexpr char lookup_table[] = "0123456789abcdef";

  inline bool is_spec_symb(const char c) {
    return
        c == '-' ||
        c == '_' ||
        c == '.' ||
        c == '~' ||
        c == '/' ||
        c == ':' ||
        c == '&' ||
        c == '?' ||
        c == '=' ;
  }

  inline bool is_alpha(const char c) {
    return
      (/* 0 */ 48 <= c && c <= /* 9 */  57) ||
      (/* A */ 65 <= c && c <= /* Z */  90) ||
      (/* a */ 97 <= c && c <= /* z */ 122) ;
  }
}

string Curl::urlencode(string& url) {
  string encoded;
  std::stringstream stream;
  for (char c : url) {
    if (is_alpha(c) || is_spec_symb(c))
    {
      stream << c;
    } else {
      stream << '%';
      stream << lookup_table[(c & 0xF0) >> 4];
      stream << lookup_table[(c & 0x0F)     ];
    }
  }
  return stream.str();
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
  return buffer;
}

string Curl::curl_gen(const string& url, const map<string, string>& params) {
  string result = url;
  for (const auto& element : params) {
    result += element.first + "=" + element.second + "&";
  }
  return result;
}

string Curl::send_request(const string& method, const map<string, string>& params) {
  return Curl::request(Curl::curl_gen(method, params));
}
