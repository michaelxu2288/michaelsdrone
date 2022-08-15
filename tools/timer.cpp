#include <timer.h>
#include <logger.h>
#include <unistd.h>
#include <chrono>

int i = 0;

auto then = std::chrono::steady_clock::now();
auto start = then;
auto now = std::chrono::steady_clock::now();

void test() {
    now = std::chrono::steady_clock::now();
    double dt = std::chrono::duration_cast<std::chrono::nanoseconds> (now - then).count() * 0.000000001;
    then = now;
    usleep(20000);
    logger::info("i: {:10d} {:10f} ", i, dt);
    i++;
}

int main() {
    timer t(test, 31);
    while(true) {
        usleep(10000);
    }
}