/**
 * @file high_speed.cpp
 * @author andrew
 * @brief runs motors at full throttle for 2s
 * @version 0.1
 * @date 2022-07-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <unistd.h>
#include <settings.h>
#include <drone.h>


int main(){

    int cycle = THROTTLE_MAX;

    std::cout << "Setting all motors to maximum speed\n";
    std::cout << "PWM signal = " << (cycle / 1000.0) << "ms\n";
    std::cout << "PWM period = 20 ms\n";
    std::cout << "!!! WARNING !!!\n";
    std::cout << "This is maximum speed. Make sure propellors are disconnected or the drone is clear of obstruction\n";
    

    
    drone d;
    d.start();
    d.set_all(cycle);

    std::cout << "Check each motor on opposite diagonals are spinning in the same direction\n and each motor on the opposite side are spinning in the other direction\n";

    std::cout << "Waiting 2 seconds\n";
    for(int i = 1; i <= 2; i ++){
        usleep(1000000);
        std::cout << i << "\n";
    }

    d.close();

    std::cout << "Motors should have rotated on success";

}