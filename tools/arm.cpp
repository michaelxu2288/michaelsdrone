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
#include <pca9685.h>
#include <unistd.h>

int main(){

    std::cout << "Arming Motors\n";

    pca9685::init();
    pca9685::set_frequency(50);
    pca9685::wake_up();
    
    pca9685::set_pwm_ms(0, THROTTLE_MIN);
    pca9685::set_pwm_ms(1, THROTTLE_MIN);
    pca9685::set_pwm_ms(2, THROTTLE_MIN);
    pca9685::set_pwm_ms(3, THROTTLE_MIN);
    usleep(2000000);
    pca9685::set_pwm_ms(0, THROTTLE_MAX);
    pca9685::set_pwm_ms(1, THROTTLE_MAX);
    pca9685::set_pwm_ms(2, THROTTLE_MAX);
    pca9685::set_pwm_ms(3, THROTTLE_MAX);
    usleep(2000000);
    pca9685::set_pwm_ms(0, THROTTLE_MIN);
    pca9685::set_pwm_ms(1, THROTTLE_MIN);
    pca9685::set_pwm_ms(2, THROTTLE_MIN);
    pca9685::set_pwm_ms(3, THROTTLE_MIN);
    usleep(1000000);

    std::cout << "Finished\n";

}