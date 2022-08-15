#include <timer.h>
#include <logger.h>
#include <unistd.h>
#include <chrono>

#include <cmath>

int i = 0;
timer t;

void test() {
    // int s = 14000 * sin(i) + 15000;
    int s = 100;
    usleep(s);
    logger::info("i: {:10d} sleep: {:10f} s freq: {:10.1f} hz ", i, s/1000000000.0, 1/t.dt);
    i++;
}

int main() {
    t.start(test, 1000/24);
    usleep(10000000);
    t.interval = 1000/48;
    usleep(10000000);
    t.interval = 1000/60;
    usleep(10000000);
}