#pragma once

#include <string_view>
#include <memory>
#include <map>

namespace curlpp
{
class Cleanup;
class Easy;
}

namespace bot
{
/*!
 * @brief The network wrapper.
 */
class Network
{
public:
  explicit
  Network();
 ~Network();
  /*!
   * @brief Make POST request.
   * @param body      - HTTP link.
   * @param params    - list of arguments.
   * @return Responce.
   */
  std::string request(std::string_view body, const std::map<std::string, std::string>& params) const;
  /*!
   * @brief Download file from <em><b>server</b></em> to <em><b>filename</b></em>.
   * @return <em><b>0</b></em> on success, <em><b>-1</b></em> otherwise.
   */
  std::size_t download(std::string_view filename, std::string_view server) const;
  /*!
   * @brief Upload file to <em><b>server</b></em> from <em><b>filename</b></em>.
   * @return Responce.
   */
  std::string upload(std::string_view filename, std::string_view server) const;
  /*!
   * @brief Make POST request with data.
   * @param body      - HTTP link.
   * @param data      - JSON or other data.
   * @return Responce.
   */
  std::string requestdata(std::string_view body, std::string_view data) const;

private:
  /*!
   * @brief CURL instance.
   */
//  void* curl_handle_;
  std::unique_ptr<curlpp::Cleanup> cleaner;
  std::unique_ptr<curlpp::Easy> curl_easy;
};
}
