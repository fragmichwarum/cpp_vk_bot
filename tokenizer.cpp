#include "tokenizer.hpp"

string_vector Tokenizer::tokenize_line(std::string line) {
    std::istringstream stream(line);
    std::vector<std::string> tokens {
        std::istream_iterator<std::string>(stream), {}
    };
    return tokens;
}
