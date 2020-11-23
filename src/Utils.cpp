#include "Utils.hpp"

using namespace bot::util;

using std::vector;
using std::string;

std::vector<std::string> bot::util::split(const std::string& text) {
  std::istringstream stream(text);
  return std::vector<std::string>{
    std::istream_iterator<std::string>(stream), { }
  };
}

string bot::util::getArgs(const string& message) {
  vector<string> splitted = split(message);
  splitted.erase(splitted.begin());
  string args = std::accumulate(splitted.begin(), splitted.end(), string{ },
                [](string& body, string& el){ return body += el + ' '; });
  args.erase(--args.end());
  return args;
}

string bot::util::emptyArgs() noexcept {
  return "Задана пустая строка.";
}

string bot::util::longToHexStr(unsigned long digit) noexcept {
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
