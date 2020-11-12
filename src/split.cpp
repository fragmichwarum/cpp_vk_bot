#include "split.hpp"

std::vector<std::string> bot::split(const std::string& text) {
  std::istringstream stream(text);
  return std::vector<std::string>{
    std::istream_iterator<std::string>(stream), { }
  };
}
