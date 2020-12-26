#pragma once

#include <string_view>
#include <vector>
#include <map>

namespace bot
{
/*!
 * @brief Collection of helper functions.
 */
namespace util
{
/*!
 * @brief Convert std::map to JSON string.
 * @param body    - content to be transformed.
 * @return JSON string.
 */
std::string toJson(const std::map<std::string, std::string>& body);
/*!
 * @param Digit to be converted.
 * @return Digit converted to hex string. WITHOUT "0x"!
 */
std::string longToHexStr(unsigned long digit);
/*!
 * @brief Split text by space.
 * @param Text to be splitted.
 * @return Vector of words.
 */
std::vector<std::string> split(std::string_view text);
/*!
 * @param The text to extract the part from.
 * @return All after first word.
 * @warning This functions does not check count of words.
 */
std::string getArgs(std::string_view message) noexcept;
/*!
 * @return Message notifying that no arguments were passed to the command.
 */
constexpr std::string_view emptyArgs() noexcept { return "Задана пустая строка."; }
/*!
 * @param user    - VK user link.
 * @return Extracted digits that represents VK user id.
 */
long extractId(std::string_view user) noexcept;
} //namespace util
} //namespace bot
