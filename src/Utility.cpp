#include <sstream>
#include <iterator>

#include "Utility.hpp"


std::string bot::util::toJson(const std::map<std::string, std::string>& body)
{
  std::string result = "{";
  bool iscomma = false;
  for (const std::pair<const std::string, std::string>& element : body) {
    if (iscomma) result += ",";
    iscomma = true;
    result += std::string{ "\"" + element.first + "\":\"" + element.second + "\"" };
  }
  return result + '}';
}

std::vector<std::string> bot::util::split(std::string_view text)
{
  std::vector<std::string> splitted;
  std::size_t start = 0;
  std::size_t end = 0;
  while ((end = text.find(' ', start)) != std::string::npos) {
    splitted.push_back(text.substr(start, end - start).data());
    start = end + 1;
  }
  splitted.push_back(text.substr(start).data());
  return splitted;
}

long bot::util::extractId(std::string_view user) noexcept
{
  /** [@id123456789|...] */
  /**    ^        ^      */
  /**    from     to     */
  return std::stol(user.substr(3, 9).data());
}

std::string bot::util::longToHexStr(unsigned long digit)
{
  static constexpr char alphabet[16] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
  };

  std::string hex;
  do {
    hex.insert(hex.begin(), alphabet[ digit % 16 ] );
  } while ((digit /= 0x10) > 0);
  return hex;
}
