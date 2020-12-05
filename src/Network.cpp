#include "Network.hpp"

#define CURL_DEBUG

bot::Network* bot::Network::instance_ = nullptr;

bot::Network::Network()
{ }

bot::Network* bot::Network::getInstance()
{
  if (instance_ != nullptr) {
    instance_ = new Network;
  }
  return instance_;
}

std::string bot::Network::urlencode(const std::string& url)
{
  char* encoded = curl_easy_escape(NULL, url.c_str(), url.length());
  std::string res{encoded};
  curl_free(encoded);
  return res;
}

size_t bot::Network::write(void* contents, size_t size, size_t nmemb, void* userp)
{
  (static_cast<std::string*>(userp))->append(static_cast<char*>(contents), size * nmemb);
  return size * nmemb;
}

size_t bot::Network::file_write(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
  return fwrite(ptr, size, nmemb, stream);
}

std::string bot::Network::toJson(const bot::traits::dictionary& body) const
{
  std::string result;
  result += '{';
  bool iscomma = false;
  for (const auto& element : body) {
    if (iscomma) {
      result += ",";
    }
    iscomma = true;
    result += std::string{ "\"" + element.first + "\":\"" + element.second + "\"" };
  }
  result += '}';
  return result;
}

std::string bot::Network::genparams(const bot::traits::dictionary& body) const
{
  std::string result;
  for (const auto& element : body) {
    result += element.first + '=' + urlencode(element.second) + '&';
  }
  return result;
}

std::string bot::Network::appendVkUrl(const std::string& method) const
{
  return "https://api.vk.com/method/" + method + '?';
}

std::string bot::Network::request(const std::string& method, const bot::traits::dictionary& params) const
{
  std::string url = method;
  url += genparams(params);
  std::string buffer;
  CURL* curl = curl_easy_init();

  if (curl) {
#if defined CURL_DEBUG
    printf("%s\n", url.c_str());
#endif
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "oxfffffe");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600L);
    curl_easy_perform(curl);
#if defined CURL_DEBUG
    printf("%s\n", buffer.c_str());
#endif
    curl_easy_cleanup(curl);
  }
  return buffer;
}

std::string bot::Network::requestdata(const std::string& method, const std::string& data) const
{
  std::string buffer;
  CURL*  curl;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, method.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_perform(curl);
  }
  curl_easy_cleanup(curl);
  return buffer;
}

std::size_t bot::Network::download(const std::string& filename, const std::string& server) const
{
  CURL* curl;
  FILE* fp;
  curl = curl_easy_init();
  if (curl) {
    fp = fopen(filename.c_str(), "wb");
    curl_easy_setopt(curl, CURLOPT_URL, server.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, file_write);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);
    if (res != CURLE_OK) {
      printf(
        "curl_easy_perform() failed: %s\b",
        curl_easy_strerror(res));
      return -1;
    }
  }
  return 0;
}

std::string bot::Network::upload(const std::string& filename, const std::string& server) const
{
  CURL* curl_handle = curl_easy_init();
  CURLcode curl_result;
  struct curl_httppost* formpost=NULL;
  struct curl_httppost* lastptr=NULL;
  std::string data;

  curl_formadd(
    &formpost,
    &lastptr,
    CURLFORM_COPYNAME, "file1",
    CURLFORM_FILENAME, filename.c_str(),
    CURLFORM_FILE, filename.c_str(),
    CURLFORM_CONTENTTYPE, "image/png",
    CURLFORM_END);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &data);
  curl_easy_setopt(curl_handle, CURLOPT_URL, server.c_str());
  curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, formpost);
  curl_result = curl_easy_perform(curl_handle);

  if (curl_result != CURLE_OK) {
     data = std::string{"curl_easy_perform() failed: "} + curl_easy_strerror(curl_result);
  }
  curl_easy_cleanup(curl_handle);
  curl_formfree(formpost);
  return data;
}
