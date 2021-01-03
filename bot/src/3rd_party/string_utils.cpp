#include "bot/include/3rd_party/string_utils.hpp"



std::string bot::third_party::string_utils::cut_first(std::string_view data)
{
  return (data.find(' ') == std::string::npos)
    ? ""
    : data.substr(data.find_first_of(' ') + 1).data();
}

std::string bot::third_party::string_utils::to_hex(unsigned long digit)
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

std::string bot::third_party::string_utils::to_json(const std::map<std::string, std::string>& body)
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
