#include <logger.h>
#include <bmp390.h>
#include <filter.h>
#include <timer.h>
#include <unistd.h>

timer t;
void loop() {
    
}

int main() {
    t.start(loop, 1000 / 24);
    while(1){
        logger::info("Data Rate: {:f} hz", t.dt);
        usleep(1000000);
    }
}