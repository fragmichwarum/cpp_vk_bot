#pragma once

#include <string_view>
#include <memory>
#include <map>

namespace curlpp
{
class Cleanup;
class Easy;
}

namespace vk::lib
{
class network
{
public:
  explicit network();
 ~network();

  std::string request(std::string_view body, const std::map<std::string, std::string>& params) const;
  std::size_t download(std::string_view filename, std::string_view server) const;
  std::string upload(std::string_view field_name, std::string_view filename, std::string_view server) const;
  std::string request_data(std::string_view body, std::string_view data) const;

private:
  std::unique_ptr<curlpp::Cleanup> cleaner;
  std::unique_ptr<curlpp::Easy> curl_easy;
};
}
