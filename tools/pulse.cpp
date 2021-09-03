#include <pwm.h>
#include <iostream>
#include <unistd.h>
#include <settings.h>
#include <drone.h>
#include <math.h>
#include <signal.h>

bool running = true;

void exit_handler(int d){
    running = false;
}


int main(){
    std::cout << "Pulsing the motor speed\n";

    int bruh = THROTTLE_MAX - (THROTTLE_MIN + 100);

    signal(SIGINT, exit_handler);

    drone d;
    d.start();
    d.enable(true, false, false, false);
    printf("fl: %d fr: %d, bl: %d br: %d\n", ENABLE_FL, ENABLE_FR, ENABLE_BL, ENABLE_BR);
    std::cout << "Check each motor on opposite diagonals are spinning in the same direction\n and each motor on the opposite side are spinning in the other direction\n";

    std::cout << "Waiting 10 seconds\n";
    double i = 0;
    while(running){
        d.set_all((int) (bruh * sin(i / 1000) + THROTTLE_MIN + 100));
        usleep(100000);
        std::cout << i << "  " << d.fl.pulse_width << "\n";
        i++;
    }

    d.close();

    std::cout << "Motors should have rotated on success";

}