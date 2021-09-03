/**
 * Arms the ESCs on the drone. Should run everytime drone is powered on.
 * 
 * Arming Sequence:
 *  min pwm - 2s
 *  max pwm - 2s
 *  min pwm - 1s
 * 
 * */
#include <iostream>
#include <settings.h>
#include <drone.h>

int main(){

    std::cout << "Arming Motors\n";

    drone d;
    d.start();
    d.arm();
    d.close();

    std::cout << "Finished\n";

}