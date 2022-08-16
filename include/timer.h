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
    double dt;
    std::function<void(void)> command;
    
    timer();
    timer(std::function<void(void)> command, unsigned int interval_ms);
    ~timer();

    void start(std::function<void(void)> command, unsigned int interval_ms);
    void start();
    void stop();

    void resume();
    void pause();
    bool isPaused();
};

// timer_thread create_timer(std::function<void(void)> command, unsigned int interval_ms);

#endif