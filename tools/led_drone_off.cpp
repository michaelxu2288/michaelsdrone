/**
 * @file led_drone_off.cpp
 * @author andrew
 * @brief Turns all leds off then turns on the error led.
 * @version 0.1
 * @date 2022-07-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <settings.h>
#include <pca9685.h>
#include <unistd.h>

int main(){

    pca9685::init();
    pca9685::set_frequency(50);
    pca9685::wake_up();
    pca9685::set_pwm_ms(LED_RUN_PIN, 0);
    pca9685::set_pwm_ms(LED_AUTO_PIN, 0);
    pca9685::set_pwm_ms(LED_WHITE_PIN, 0);
    pca9685::set_pwm_ms(LED_BLUE_PIN, 0);
    pca9685::set_pwm_ms(LED_ERROR_PIN, PWM_FULL);
    // pca9685::sleep();
    pca9685::destroy();
}