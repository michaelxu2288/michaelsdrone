#include <timer.h>
#include <chrono>

timer::timer() {
    running = false;
}
static void bruh(std::function<void(void)> & command, unsigned int & interval, std::mutex & m){
    auto chrono_interval = std::chrono::milliseconds(interval);
    while(running) {
        std::lock_guard <std::mutex> lock(thread_mutex);
        auto next = std::chrono::steady_clock::now() + chrono_interval;
        command();
        std::this_thread::sleep_until(next);
    }
}
timer::timer(std::function<void(void)> _command, unsigned int _interval_ms) {
    running = true;
    command = _command;
    interval = _interval_ms;
    thread = std::thread(bruh, command, interval, thread_mutex);
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