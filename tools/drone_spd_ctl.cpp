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

int main(){


    pca9685::init();
    pca9685::set_frequency(50);
    pca9685::wake_up();
    
    double power = 0;
    while(power >= 0){
        pca9685::set_pwm_ms(0, THROTTLE_MIN + (int) (power / 100 * (THROTTLE_MAX - THROTTLE_MIN)));
        pca9685::set_pwm_ms(1, THROTTLE_MIN + (int) (power / 100 * (THROTTLE_MAX - THROTTLE_MIN)));
        pca9685::set_pwm_ms(2, THROTTLE_MIN + (int) (power / 100 * (THROTTLE_MAX - THROTTLE_MIN)));
        pca9685::set_pwm_ms(3, THROTTLE_MIN + (int) (power / 100 * (THROTTLE_MAX - THROTTLE_MIN)));

        std::cout << "Power %: ";
        std::cin >> power;
        if(power > 100.0){
            power = 100.0;
        }
    }
        pca9685::set_pwm_ms(0, THROTTLE_MIN);
        pca9685::set_pwm_ms(1, THROTTLE_MIN);
        pca9685::set_pwm_ms(2, THROTTLE_MIN);
        pca9685::set_pwm_ms(3, THROTTLE_MIN);

    std::cout << "Finished\n";

}