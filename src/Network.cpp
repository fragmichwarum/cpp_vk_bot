#include "Network.hpp"

//#define CURL_DEBUG

bot::Network::~Network()
{
  curl_easy_cleanup(curl_handle_);
}

static std::string escape(std::string_view url)
{
  char* encoded = curl_easy_escape(NULL, url.data(), url.length());
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
    result += element.first + '=' + escape(element.second) + '&';
  }
  return result;
}

std::string bot::Network::request(std::string_view method, const bot::dictionary& params) const noexcept
{
  std::string url = method.data() + genparams(params);
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
    printf("buffer: %s\n", buffer.data());
#endif
  }
  return buffer;
}

std::string bot::Network::requestdata(std::string_view method, std::string_view data) const noexcept
{
  std::string buffer;

  if (curl_handle_) {
    curl_easy_setopt(curl_handle_, CURLOPT_URL, method.data());
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, write);
    curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDSIZE, data.size());
    curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, data.data());
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &buffer);
    curl_easy_perform(curl_handle_);
    curl_easy_reset(curl_handle_);
  }
  return buffer;
}

std::size_t bot::Network::download(std::string_view filename, std::string_view server) const noexcept
{
  FILE* fp;

  if (curl_handle_) {
    fp = fopen(filename.data(), "wb");
    curl_easy_setopt(curl_handle_, CURLOPT_URL, server.data());
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

std::string bot::Network::upload(std::string_view filename, std::string_view server) const noexcept
{
  CURLcode curl_result;
  struct curl_httppost* formpost = NULL;
  struct curl_httppost* lastptr = NULL;
  std::string data;

  curl_formadd(
    &formpost,
    &lastptr,
    CURLFORM_COPYNAME, "file1",
    CURLFORM_FILENAME, filename.data(),
    CURLFORM_FILE, filename.data(),
    CURLFORM_CONTENTTYPE, "image/png",
    CURLFORM_END);
  curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, write);
  curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &data);
  curl_easy_setopt(curl_handle_, CURLOPT_URL, server.data());
  curl_easy_setopt(curl_handle_, CURLOPT_HTTPPOST, formpost);
  curl_result = curl_easy_perform(curl_handle_);
  curl_easy_reset(curl_handle_);

  if (curl_result != CURLE_OK) {
     data = std::string{"curl_easy_perform() failed: "} + curl_easy_strerror(curl_result);
  }
  curl_formfree(formpost);
  return data;
}
