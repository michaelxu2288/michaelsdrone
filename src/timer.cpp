#include <timer.h>
#include <chrono>

timer::timer() {
    running = false;
}

timer::timer(std::function<void(void)> _command, unsigned int _interval_ms) {
    running = true;
    command = _command;
    interval = _interval_ms;
    thread = std::thread(this);
}

void timer::operator() (){
    auto chrono_interval = std::chrono::milliseconds(interval);
    while(running) {
        std::lock_guard <std::mutex> lock(thread_mutex);
        auto next = std::chrono::steady_clock::now() + chrono_interval;
        command();
        std::this_thread::sleep_until(next);
    }
}

timer::~timer() {
    running = false;
}