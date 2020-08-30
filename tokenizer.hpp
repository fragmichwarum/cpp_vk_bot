#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iterator>

using string_vector = std::vector<std::string>;

class Tokenizer
{
public:
    static string_vector tokenize_line(std::string line);
};

#endif // TOKENIZER_HPP
