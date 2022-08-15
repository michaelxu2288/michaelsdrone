#include <timer.h>
#include <chrono>

timer::timer() {
    running = false;
}

static void bruh(std::function<void(void)> command, bool running, unsigned int interval){
    auto chrono_interval = std::chrono::milliseconds(interval);
    while(running) {
        // std::lock_guard <std::mutex> lock(m);
        auto next = std::chrono::steady_clock::now() + chrono_interval;
        command();
        // while(next < std::chrono::steady_clock::now()){
        //     next += chrono_interval;
        // }
        std::this_thread::sleep_until(next);
    }
}

timer::timer(std::function<void(void)> _command, unsigned int _interval_ms) {
    running = true;
    command = _command;
    interval = _interval_ms;
    thread = std::thread(bruh, command, running, interval);
}

timer::~timer() {
    running = false;
}