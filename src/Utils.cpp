#include "Utils.hpp"

std::vector<std::string> bot::util::split(const std::string& text)
{
  std::istringstream stream(text);
  return std::vector<std::string>{
    std::istream_iterator<std::string>(stream), { }
  };
}

std::string bot::util::getArgs(const std::string& message)
{
  std::vector<std::string> splitted = split(message);
  splitted.erase(splitted.begin());
  std::string args = std::accumulate(splitted.begin(), splitted.end(), std::string{ },
                [](std::string& body, std::string& el){ return body += el + ' '; });
  args.erase(--args.end());
  return args;
}

std::string bot::util::emptyArgs() noexcept
{
  return "Задана пустая строка.";
}

std::string bot::util::longToHexStr(unsigned long digit) noexcept
{
  static constexpr char const alphabet[0x10] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
  };

  std::string hex;
  do {
    hex.insert(hex.begin(), alphabet[ digit % 0x10 ] );
  } while ((digit /= 0x10) > 0);
  return hex;
}
