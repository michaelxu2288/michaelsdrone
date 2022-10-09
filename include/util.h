#ifndef UTIL_H
#define UTIL_H

#include <string_view>
#include <vector>
#include <string>
namespace cstr {
    size_t index_of(const char * in, const char * delim);
    size_t index_of(const char * in, const char * delim, const size_t s);
    size_t substr(const char * in, size_t start, size_t end);
    void shift(char * in, size_t amount, char * out);
    void split(const char * in, const char delim, std::vector<std::string> out);
}

namespace str {
    void split(const std::string_view in, const char delim, std::vector<std::string> out);
}

#endif