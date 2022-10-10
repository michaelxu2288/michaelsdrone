#include <util.h>
#include <cstdio>
// #ifndef UTIL_H
// #define UTIL_H

// #include <string_view>
// #include <vector>
// #include <string>
// namespace cstr {
//     ssize_t index_of(const char * in, const char * delim);
//     size_t substr(const char * in, size_t start, size_t end);
//     size_t shift(char * in, size_t amount, char * out);
// }

// namespace string {
//     void split(const std::string_view in, const char delim, std::vector<std::string> out);
// }

// #endif

bool has(const char * in, const char * delim) {
    for(size_t i = 0; delim[i] != '\0'; i ++) {
        if(in[i] == '\0') {
            return false;
        }
        if(in[i] != delim[i]) {
            return false;
        }
    }
    return true;
}

size_t cstr::index_of(const char * in, const char * delim, const size_t s) {
    for(size_t i = s; in[i] != '\0'; i++) {
        if(has(in + i, delim)) {
            return i;
        }
    }
    return -1;
}

size_t cstr::index_of(const char * in, const char * delim) {
    for(size_t i = 0; in[i] != '\0'; i++) {
        if(has(in + i, delim)) {
            return i;
        }
    }
    return -1;
}

size_t cstr::substr(const char * in, size_t start, size_t end) {

}
void cstr::shift(char * in, size_t amount, char * out) {
    size_t i = 0;
    for(i = 0; i < amount; i ++) {
        out[i] = in[i];
    }
    out[amount] = '\0';
    for(i = 0; in[i + amount] != '\0'; i ++) {
        in[i] = in[i + amount];
    }
    in[i] = '\0';
}


void cstr::split(const char * in, const char * delim, size_t delim_length, std::vector<std::string> out) {
    printf("%s\n", in);
    size_t next = cstr::index_of(in, delim, 0);
    size_t last = 0;
    printf("%d %d\n", last, next);
    while(next >= 0) {
        if(next - last > 0){
            std::string str(in + last, next - last);
            printf("%s\n", str.c_str());
            out.push_back(str);
        } 
        last = next + 2;
        next = cstr::index_of(in, delim, last);
        printf("%d %d\n", last, next);
    }
}