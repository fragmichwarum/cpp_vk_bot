#include "curl.hpp"

using std::string;

string cURL::urlencode(const string& url) {
  return curl_easy_escape(NULL, url.c_str(), url.length());
}

size_t write_callback(
  void* contents,
  size_t size,
  size_t nmemb,
  void* userp)
{
  (static_cast<string*>(userp))->append(static_cast<char*>(contents), size * nmemb);
  return size * nmemb;
}

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

string cURL::request(const string& method, const params& body) {
  string url = method;
  url += genparams(body);
  string buffer;
  CURL*  curl;
  curl = curl_easy_init();
  printf("%s\n", url.c_str());
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "oxfffffe");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600L);
    curl_easy_perform(curl);
    printf("%s\n", buffer.c_str());
  }
  curl_easy_cleanup(curl);
  return buffer;
}

string cURL::genparams(const params& body) {
  string result;
  for (const auto& element : body) {
    result += urlencode(element.first) + '=' + urlencode(element.second) + '&';
  }
  return result;
}

void cURL::append_vkparams(params& map) {
  map["random_id"] = "0";
  map["access_token"] = access_token;
  map["v"] = api_version;
}

string cURL::append_vkurl(const string &method) {
  return api_url + method + '?';
}
