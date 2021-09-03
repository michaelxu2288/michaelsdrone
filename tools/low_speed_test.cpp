#include <pwm.h>
#include <iostream>
#include <unistd.h>
#include <settings.h>
#include <drone.h>


int main(){

    int cycle = 1100;

    std::cout << "Setting all motors to a very low speed\n";
    std::cout << "PWM signal = " << (cycle / 1000.0) << "ms\n";
    std::cout << "PWM period = 20 ms";
    drone d;
    d.set_all(cycle);
    d.start();

    std::cout << "Check each motor on opposite diagonals are spinning in the same direction\n and each motor on the opposite side are spinning in the other direction\n";

    std::cout << "Waiting 10 seconds\n";
    for(int i = 1; i <= 10; i ++){
        usleep(1000000);
        std::cout << i << "\n";
    }

    d.close();

    std::cout << "Motors should have rotated on success";

}