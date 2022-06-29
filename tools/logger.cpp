#include <logger.h>
// #include <iostream>
// #include <cmath>
#include <cstdio>
#include <source_location>


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
}