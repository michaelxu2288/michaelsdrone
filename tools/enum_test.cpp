#include <logger.h>


enum test {
    cock,
    balls,
    dick,
};

int main(){
    test s = cock;
    logger::info("1: {}", s);
    test * k = &s;
    logger::info("2: {}", *k);
    int * j = (int *) k;
    logger::info("3: {}", *j);
}