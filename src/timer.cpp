#include <timer.h>
#include <chrono>

timer::timer() {
    running = false;
}

static void bruh(timer * t/* std::function<void(void)> command, bool running, unsigned int interval */){
    auto now = std::chrono::steady_clock::now();
    auto then = std::chrono::steady_clock::now();
    while(t->running) {
        now = std::chrono::steady_clock::now();
        t->dt = std::chrono::duration_cast<std::chrono::nanoseconds> (now - then).count() * 0.000000001;
        then = now;

        std::lock_guard <std::mutex> lock(t->thread_mutex);
        auto next = std::chrono::steady_clock::now() + std::chrono::milliseconds(t->interval);
        t->command();
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
    thread = std::thread(bruh, this);
}

void start(std::function<void(void)> _command, unsigned int _interval_ms) {
    running = true;
    command = _command;
    interval = _interval_ms;
    thread = std::thread(bruh, this);
}

timer::~timer() {
    running = false;
}