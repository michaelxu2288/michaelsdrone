#include <logger.h>
// #include <iostream>
// #include <cmath>
#include <cstdio>


void foobar(const char* msg, const char* str = __builtin_FUNCTION()){
    std::printf("%s (called by %s)\n", msg, str);
}


int main(){
    // std::cout << "bruh\n";
    // printf("bfuabfdsaj\n");
    // printf("%s\n", _SOURCE_LOCATION_);
    foobar("this is a message");
    logger::debug("chugging along");
    logger::info("theres something you need to know!");
    logger::warn("expected a 0, but got a 1");
    logger::crit("FIRE FIRE FIRE");
    logger::err("something went wrong");

    logger::debug("testing formatting {}.", "test worked!");
    logger::info("testing formatting {}.", "test worked!");
    logger::warn("testing formatting {}.", "test worked!");
    logger::crit("testing formatting {}.", "test worked!");
    logger::err("testing formatting {}.", "test worked!");
}