#include <logger.h>
#include <unistd.h>
#include <fcntl.h>
#include <util.h>
#include <string>

#define INTERNAL_BUFFER 1024

int main() {
    
    std::string test = "ab !bc !cd !de !ef !fg !gh !hi !ij";
    std::vector<std::string> strs;
    // cstr::split(buf, " !", 2, strs);
    str::split(test, " !", strs);
    for(int i = 0; i < strs.size(); i ++) {
        logger::info("Cock: {}", strs[i]);
    }

}