#pragma once

#include <curl/curl.h>
#include <map>
#include <string>

namespace bot
{
/*!
 * @namespace bot::cURL
 * @brief Namespace with functions for working with the network.
 */
namespace cURL
{
/*!
 * @brief Converts string to ASCII character-set.
 * @param Url
 * @return Encoded string.
 */
std::string urlencode(const std::string& url);
/*!
 * @brief Transforms method name to VK API url.
 * @param Method name
 * @return cURL-ready method name.
 */
std::string append_vkurl(const std::string& method);
/*!
 * @brief Converts std::map<std::string, std::string> to JSON format.
 * @param Map
 * @return JSON string.
 */
std::string to_json(const std::map<std::string, std::string>& body);
/*!
 * @brief Executes the POST request.
 * @param HTTP body.
 * @param HTTP parameters.
 * @return JSON or XML output.
 */
std::string request(const std::string& body, const std::map<std::string, std::string>& params);
/*!
 * @brief Sends data via POST.
 * @param HTTP body.
 * @param HTTP parameters.
 * @return JSON or XML output.
 */
std::string requestdata(std::string body, const std::string& data);
} //namespace cURL
} //namespace bot
