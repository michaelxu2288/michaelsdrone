#include <timer.h>
#include <logger.h>
#include <unistd.h>

int i = 0;
void test() {
    logger::info("i: {:d}", i);
    i++;
}

int main() {
    timer t(test, 1000);
    while(true) {
        usleep(10000);
    }
}