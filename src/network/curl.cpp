#include <sstream>
#include <curl/curl.h>
#include "curl.hpp"

using std::string;
using std::stringstream;

namespace {
string char_to_hex(const char c) {
  stringstream stream;
  if (static_cast<int>(c) < 0x10) {
    stream << "0";
  }
  stream << std::hex << static_cast<int>(c);
  return '%' + stream.str();
}

string urlencode(string& url) {
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

size_t write_callback(
  void* contents,
  size_t size,
  size_t nmemb,
  void* userp)
{
  ((string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}
} /* namespace */

string cURL::to_json(const params& body) {
  string result;
  result += '{';
  bool iscomma = false;
  for (const auto& element : body) {
    if (iscomma) {
      result += ",";
    }
    iscomma = true;
    result += "\"";
    result += element.first;
    result += "\":\"";
    result += element.second;
    result += "\"";
  }
  result += '}';
  return result;
}

string cURL::requestdata(string method, const string& data) {
  string buffer;
  CURL*  curl;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, method.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_perform(curl);
  }
  curl_easy_cleanup(curl);
  return buffer;
}

string cURL::request(string method, const params& body) {
  string url = method;
  url += genparams(body);
  string buffer;
  CURL*  curl;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, urlencode(url).c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600L);
      curl_easy_perform(curl);
  }
  printf("%s\n", buffer.c_str());
  curl_easy_cleanup(curl);
  return buffer;
}

string cURL::genparams(const params& body) {
  string result;
  for (const auto& element : body) {
    result += element.first + '=' + element.second + '&';
  }
  return result;
}

void cURL::append_vkparams(params& map) {
  map["access_token"] = access_token;
  map["v"] = api_version;
}

string cURL::append_vkurl(const string &method) {
  return api_url + method + '?';
}
