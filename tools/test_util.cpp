#include <logger.h>
#include <unistd.h>
#include <fcntl.h>
#include <util.h>
#include <string>

#define INTERNAL_BUFFER 1024

int main() {
    
    char buf[INTERNAL_BUFFER];
    char buf2[INTERNAL_BUFFER];
    int len = 0;

    strcpy(buf, "hello world!");
    int i = cstr::index_of(buf, "wo");
    logger::info("{}", i);
    cstr::shift(buf, i+2, buf2);
    logger::info("{}", buf2);
    logger::info("{}", buf);

    strcpy(buf, "hello world!");
    i = cstr::index_of(buf, "w ");
    logger::info("{}", i);
    
    strcpy(buf, "ab !bc !cd !de !ef !fg !gh !hi !ij");
    std::string test = "ab !bc !cd !de !ef !fg !gh !hi !ij";
    std::vector<std::string> strs;
    // cstr::split(buf, " !", 2, strs);
    str::split(test, " !", strs);
    for(i = 0; i < strs.size(); i ++) {
        logger::info("Cock: {}", strs[i]);
    }

}