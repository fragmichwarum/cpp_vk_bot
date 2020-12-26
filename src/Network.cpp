#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <sstream>

#include "Network.hpp"

#define CURL_DEBUG

bot::Network::Network()
  : cleaner(std::make_unique<curlpp::Cleanup>())
  , curl_easy(std::make_unique<curlpp::Easy>())
{ }

bot::Network::~Network() = default;

static std::string escape(std::string_view url)
{
  char* encoded = curl_easy_escape(NULL, url.data(), url.length());
  std::string res{encoded};
  curl_free(encoded);
  return res;
}

static size_t file_write(FILE *f, char* ptr, size_t size, size_t nmemb)
{
  return fwrite(ptr, size, nmemb, f);
}

static std::string genparams(const std::map<std::string, std::string>& body)
{
  std::string result;
  for (const auto& element : body) {
    result += element.first + '=' + escape(element.second) + '&';
  }
  return result;
}

std::string bot::Network::request(std::string_view method, const std::map<std::string, std::string>& params) const
{
  std::string url = method.data() + genparams(params);
  std::ostringstream response;

  curl_easy->setOpt(new curlpp::options::Url(url));
  curl_easy->setOpt(new curlpp::options::WriteStream(&response));
  curl_easy->perform();

  return response.str();
}

std::string bot::Network::requestdata(std::string_view method, std::string_view data) const
{
  std::ostringstream response;

  curl_easy->setOpt(new curlpp::options::Url(method.data()));
  curl_easy->setOpt(new curlpp::options::PostFields(data.data()));
  curl_easy->setOpt(new curlpp::options::PostFieldSize(data.size()));
  curl_easy->setOpt(new curlpp::options::WriteStream(&response));
  curl_easy->perform();

  return response.str();
}

std::size_t bot::Network::download(std::string_view filename, std::string_view server) const
{
  FILE* fp = fopen(filename.data(), "w");
  if (not fp) return -1;

  curlpp::options::WriteFunction* writef
    = new curlpp::options::WriteFunction(std::bind(
      &file_write, fp,
      std::placeholders::_1,
      std::placeholders::_2,
      std::placeholders::_3));
  curl_easy->setOpt(writef);
  curl_easy->setOpt(new curlpp::options::Url(server.data()));
  curl_easy->perform();
  return 0;
}

std::string bot::Network::upload(std::string_view filename, std::string_view server) const
{
  std::ostringstream response;
  curlpp::Forms formParts;
  formParts.push_back(new curlpp::FormParts::File("file1", filename.data()));

  curl_easy->setOpt(new curlpp::options::Url(server.data()));
  curl_easy->setOpt(new curlpp::options::HttpPost(formParts));
  curl_easy->setOpt(new curlpp::options::WriteStream(&response));
  curl_easy->perform();
  curl_easy->reset();

  return response.str();
}
