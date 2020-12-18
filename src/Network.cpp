#include "Network.hpp"

#define CURL_DEBUG

bot::Network::~Network()
{
  curl_easy_cleanup(curl_handle_);
}

static inline std::string urlencode(const std::string& url)
{
  char* encoded = curl_easy_escape(NULL, url.c_str(), url.length());
  std::string res{encoded};
  curl_free(encoded);
  return res;
}

static size_t write(void* contents, size_t size, size_t n, void* userp)
{
  (static_cast<std::string*>(userp))->append(static_cast<char*>(contents), size * n);
  return size * n;
}

static size_t file_write(void* ptr, size_t size, size_t n, FILE* stream)
{
  return fwrite(ptr, size, n, stream);
}

static std::string genparams(const bot::dictionary& body)
{
  std::string result;
  for (const auto& element : body) {
    result += element.first + '=' + urlencode(element.second) + '&';
  }
  return result;
}

std::string bot::Network::request(const std::string& method, const bot::dictionary& params) const
{
  std::string url = method + genparams(params);
  std::string buffer;

  if (curl_handle_) {
#if defined CURL_DEBUG
    printf("source: %s\n", url.c_str());
#endif
    curl_easy_setopt(curl_handle_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, write);
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl_handle_, CURLOPT_USERAGENT, "oxfffffe");
    curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT, 600L);
    curl_easy_perform(curl_handle_);
    curl_easy_reset(curl_handle_);
#if defined CURL_DEBUG
    printf("buffer: %s\n", buffer.c_str());
#endif
  }
  return buffer;
}

std::string bot::Network::requestdata(const std::string& method, const std::string& data) const
{
  std::string buffer;

  if (curl_handle_) {
    curl_easy_setopt(curl_handle_, CURLOPT_URL, method.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, write);
    curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDSIZE, data.size());
    curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &buffer);
    curl_easy_perform(curl_handle_);
    curl_easy_reset(curl_handle_);
  }
  return buffer;
}

std::size_t bot::Network::download(const std::string& filename, const std::string& server) const
{
  FILE* fp;

  if (curl_handle_) {
    fp = fopen(filename.c_str(), "wb");
    curl_easy_setopt(curl_handle_, CURLOPT_URL, server.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, file_write);
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, fp);
    CURLcode res = curl_easy_perform(curl_handle_);
    curl_easy_reset(curl_handle_);
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
  CURLcode curl_result;
  struct curl_httppost* formpost = NULL;
  struct curl_httppost* lastptr = NULL;
  std::string data;

  curl_formadd(
    &formpost,
    &lastptr,
    CURLFORM_COPYNAME, "file1",
    CURLFORM_FILENAME, filename.c_str(),
    CURLFORM_FILE, filename.c_str(),
    CURLFORM_CONTENTTYPE, "image/png",
    CURLFORM_END);
  curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, write);
  curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &data);
  curl_easy_setopt(curl_handle_, CURLOPT_URL, server.c_str());
  curl_easy_setopt(curl_handle_, CURLOPT_HTTPPOST, formpost);
  curl_result = curl_easy_perform(curl_handle_);
  curl_easy_reset(curl_handle_);

  if (curl_result != CURLE_OK) {
     data = std::string{"curl_easy_perform() failed: "} + curl_easy_strerror(curl_result);
  }
  curl_formfree(formpost);
  return data;
}
