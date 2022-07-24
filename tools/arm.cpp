/**
 * @file arm.cpp
 * @author andrew
 * @brief Arms the escs.
 * 
 * throttle min - 2s
 * throttle max - 2s
 * throttle min - 2s
 * 
 * @version 0.1
 * @date 2022-07-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <settings.h>
#include <pca9685.h>
#include <unistd.h>
#include <logger.h>
// #include <INIReader.h>
#include <config.h>

int main(){

    // std::cout << "Arming Motors\n";

    // INIReader reader(DRONE_STATE_CONFIG_FILE);
    // config::load_file()
    config::config drone_state(DRONE_STATE_CONFIG_FILE);
    bool armed = drone_state.get_config_bool("armed", false);
    if(armed) {
        logger::warn("Arming sequence already performed.");
        return 0;
    }
    drone_state.set("armed", true);
    drone_state.write_to_file(DRONE_STATE_CONFIG_FILE);

#ifdef ENABLE_MOTOR
    logger::info("Performing arming sequence.");

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

    logger::info("Finished arming sequence.");
#else
    logger::info("Motors are not enabled!");
#endif
    // std::cout << "Finished\n";

}