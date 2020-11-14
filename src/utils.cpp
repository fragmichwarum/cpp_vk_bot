#include <numeric>

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

string bot::util::empty_args()
{
  return "Задана пустая строка.";
}
