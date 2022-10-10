#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
namespace cstr {
    size_t index_of(const char * in, const char * delim);
    size_t index_of(const char * in, const char * delim, const size_t s);
}

namespace str {
    // void split(const std::string_view in, const char delim, std::vector<std::string> out);
    void split(const std::string in, const std::string delim, std::vector<std::string> out);
}

#endif