#include "utils.hpp"

using namespace bot::util;

using std::vector;
using std::string;

std::vector<std::string> bot::util::split(const std::string& text) {
  std::istringstream stream(text);
  return std::vector<std::string>{
    std::istream_iterator<std::string>(stream), { }
  };
}

string bot::util::get_args(const string& message)
{
  vector<string> splitted = split(message);
  splitted.erase(splitted.begin());
  return std::accumulate(splitted.begin(), splitted.end(), string{ },
                         [](string& body, string& el){ return body += el + ' '; });
}

string bot::util::empty_args() noexcept(true)
{
  return "Задана пустая строка.";
}

string bot::util::long_to_hex_str(unsigned long digit) noexcept(true) {
  static constexpr char const alphabet[0x10] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
  };

  string hex;
  do {
    hex.insert(hex.begin(), alphabet[ digit % 0x10 ] );
  } while ((digit /= 0x10) > 0);
  return hex;
}
