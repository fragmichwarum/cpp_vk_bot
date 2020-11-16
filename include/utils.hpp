#pragma once

#include <vector>
#include <numeric>
#include <sstream>
#include <iterator>

namespace bot
{
/*!
 * @namespace bot::util
 * @brief Namespace, that contains utilities for
 * convenient work with @ref bot::Cmd_handler and @ref bot::Vk_api.
 */
namespace util
{
/*!
 * @brief Convert number to hex string.
 * @param[in] digit - unsigned number to be transofmed.
 * @return Number converted into hex string.
 */
std::string long_to_hex_str(unsigned long digit) noexcept(true);
/*!
 * @brief Split string by spaces.
 * @param text - string to be transformed.
 * @return Splitted string represented as std::vector<std::string>.
 */
std::vector<std::string> split(const std::string& text);
/*!
 * @brief Get all words from second to end of string.
 * @param text
 * @return String with cutted first word.
 */
std::string get_args(const std::string& message);
/*!
 * @brief Returns message if the command received no arguments.
 * @return No arguments message.
 */
std::string empty_args() noexcept(true);
} // util
} // bot
