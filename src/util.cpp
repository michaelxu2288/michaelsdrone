#include <util.h>
#include <cstdio>

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

void cstr::split(const char * in, const char * delim, size_t delim_length, std::vector<std::string> out) {
    printf("%s\n", in);
    size_t next = cstr::index_of(in, delim, 0);
    size_t last = 0;
    printf("%d %d\n", last, next);
    while(next != -1) {
        printf("FUCK  FUANDFSKFDS  - %d %d\n", last, next);
        if(next - last > 0){
            std::string str(in + last, next - last);
            printf("%s\n", str.c_str());
            out.push_back(str);
        } 
        last = next + 2;
        next = cstr::index_of(in, delim, last);
        printf("%d %d\n", last, next);
    }
    if(in[last] != '\0') {
        std::string str(in + last);
        out.push_back(str);
    }
}

void str::split(const std::string in, const std::string delim, std::vector<std::string> out) {
    
    auto start = 0U;
    auto end = in.find(delim);
    while (end != std::string::npos)
    {
        // std::cout << s.substr(start, end - start) << std::endl;
        out.push_back(in.substr(start, end - start));
        start = end + delim.length();
        end = in.find(delim, start);
    }
    out.push_back(in.substr(start, end));
}