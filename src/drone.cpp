#include <drone.h>
#include <pca9685.h>
#include <unistd.h>
#include <csignal>
#include <cstdio>
#include <stdlib.h>

void clear_led(){
    pca9685::set_pwm_ms(LED_RUN_PIN, 0);
    pca9685::set_pwm_ms(LED_AUTO_PIN, 0);
    pca9685::set_pwm_ms(LED_ERROR_PIN, 0);
    pca9685::set_pwm_ms(LED_WHITE_PIN, 0);
    pca9685::set_pwm_ms(LED_BLUE_PIN, 0);
}
void drone::force_terminate(){
    clear_led();
    pca9685::set_pwm_ms(LED_RUN_PIN, 0);
    pca9685::set_pwm_ms(LED_ERROR_PIN, PWM_FULL-1);
    drone::set_all(0);
}


void terminate_handle(int signum){
    drone::force_terminate();
    
    exit(0);
}


void drone::init(){
    pca9685::set_frequency(50);
    pca9685::init();
    pca9685::wake_up();
    pca9685::set_pwm_ms(LED_RUN_PIN, PWM_FULL - 1);
    signal(SIGINT, terminate_handle);
    clear_led();
}

void drone::arm(){

}

void drone::set_all(double per){
    int pow = (int)(per * (THROTTLE_MAX - THROTTLE_MIN)) + THROTTLE_MIN;
    pca9685::set_pwm_ms(MOTOR_FL_PIN, pow);
    pca9685::set_pwm_ms(MOTOR_FR_PIN, pow);
    pca9685::set_pwm_ms(MOTOR_BL_PIN, pow);
    pca9685::set_pwm_ms(MOTOR_BR_PIN, pow);
}

void drone::set_diagonals(short diagonal, double per){
    int pow = (int)(per * (THROTTLE_MAX - THROTTLE_MIN)) + THROTTLE_MIN;
    switch (diagonal)
    {
    case FLBR_DIAGONAL:
        pca9685::set_pwm_ms(MOTOR_FL_PIN, pow);
        pca9685::set_pwm_ms(MOTOR_BR_PIN, pow);
        
        break;
    case FRBL_DIAGONAL:
        pca9685::set_pwm_ms(MOTOR_FR_PIN, pow);
        pca9685::set_pwm_ms(MOTOR_BL_PIN, pow);
        break;
    
    default:
        break;
    }
}

void drone::set_motor(short motor, double per){
    int pow = (int)(per * (THROTTLE_MAX - THROTTLE_MIN)) + THROTTLE_MIN;
    switch(motor){
    case MOTOR_FL:
        pca9685::set_pwm_ms(MOTOR_FL_PIN, pow);
        break;
    case MOTOR_FR:
        pca9685::set_pwm_ms(MOTOR_FR_PIN, pow);
        break;
    case MOTOR_BL:
        pca9685::set_pwm_ms(MOTOR_BL_PIN, pow);
        break;
    case MOTOR_BR:
        pca9685::set_pwm_ms(MOTOR_BR_PIN, pow);
        break;
    default:break;
    }
}

void drone::destroy(){
    
    set_all(0);

    pca9685::destroy();
}