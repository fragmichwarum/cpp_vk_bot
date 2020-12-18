#include <sstream>
#include <iterator>

#include "Utility.hpp"

std::string bot::util::toJson(const std::map<std::string, std::string>& body)
{
  std::string result;
  result += '{';
  bool iscomma = false;
  for (const std::pair<const std::string, std::string>& element : body) {
    if (iscomma) {
      result += ",";
    }
    iscomma = true;
    result += std::string{ "\"" + element.first + "\":\"" + element.second + "\"" };
  }
  result += '}';
  return result;
}

std::vector<std::string> bot::util::split(const std::string& text)
{
  std::istringstream stream(text);
  return std::vector<std::string>{
    std::istream_iterator<std::string>(stream), { }
  };
}

std::string bot::util::getArgs(const std::string& message)
{
  return message.substr(message.find_first_of(" ") + 1);
}

long bot::util::extractId(const std::string& user)
{
  /** [@id123456789|...] */
  /**    ^        ^      */
  /**    from     to     */
  return stol(user.substr(3, 9));
}

std::string bot::util::emptyArgs() noexcept
{
  return "Задана пустая строка.";
}

std::string bot::util::longToHexStr(unsigned long digit) noexcept
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
