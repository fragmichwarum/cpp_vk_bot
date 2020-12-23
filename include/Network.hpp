#pragma once

#include <curl/curl.h>
#include <string>
#include <map>

namespace bot
{
using dictionary = std::map<std::string, std::string>;

/*!
 * @brief The network wrapper.
 */
class Network
{
private:
  /*!
   * @brief CURL instance.
   */
  CURL* curl_handle_ = curl_easy_init();

public:
 ~Network();
  /*!
   * @brief Make POST request.
   * @param body      - HTTP link.
   * @param params    - list of arguments.
   * @return Responce.
   */
  std::string request(std::string_view body, const dictionary& params) const;
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
};
}
