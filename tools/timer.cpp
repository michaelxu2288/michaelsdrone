#include <timer.h>
#include <logger.h>
#include <unistd.h>
#include <chrono>

#include <cmath>

int i = 0;

auto then = std::chrono::steady_clock::now();
auto start = then;
auto now = std::chrono::steady_clock::now();

void test() {
    now = std::chrono::steady_clock::now();
    double dt = std::chrono::duration_cast<std::chrono::nanoseconds> (now - then).count() * 0.000000001;
    then = now;
    int s = 14000 * sin(i) + 15000;
    usleep(s);
    logger::info("i: {:10d} sleep: {:10f} s freq: {:10.1f} hz ", i, s/1000000000.0, 1/dt);
    i++;
}

int main() {
    timer t(test, 1000/24);
    usleep(10000000);
    t.interval = 1000/48;
    usleep(10000000);
    t.interval = 1000/60;
    usleep(10000000);
}