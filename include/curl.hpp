#pragma once

#include <curl/curl.h>
#include <map>
#include "metadata.hpp"

namespace bot
{
/*!
 * @brief Version of VK API.
 */
inline const std::string api_version = "5.124";
/*!
 * @brief VK API URL.
 */
inline const std::string api_url = "https://api.vk.com/method/";
/*!
 * @namespace bot::cURL
 * @brief Namespace, that contains functions for working with the network.
 */
namespace cURL
{
/*!
 * @brief Convert string to ASCII character-set.
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
 * @brief Convert std::map<std::string, std::string> to JSON format.
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
