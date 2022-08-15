#ifndef TIMER_H_GUARD
#define TIMER_H_GUARD

#include <functional>
#include <thread>
#include <mutex>

struct timer {
    bool running; // Change this to stop the timer
    std::thread thread; // Underlying thread
    unsigned int interval; // Time interval in ms 
    std::mutex thread_mutex;
    std::function<void(void)> command;
    timer();
    timer(std::function<void(void)> command, unsigned int interval_ms);
    ~timer();
};

// timer_thread create_timer(std::function<void(void)> command, unsigned int interval_ms);

#endif