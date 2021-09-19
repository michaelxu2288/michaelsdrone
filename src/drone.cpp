#include <drone.h>
#include <pca9685.h>


void drone::init(){
    pca9685::init();

}

void drone::arm(){

}

void drone::set_all(double per){
    int pow = per * (THROTTLE_MAX - THROTTLE_MIN) + THROTTLE_MIN;
    pca9685::set_pwm_ms(MOTOR_FL, pow);
    pca9685::set_pwm_ms(MOTOR_FR, pow);
    pca9685::set_pwm_ms(MOTOR_BL, pow);
    pca9685::set_pwm_ms(MOTOR_BR, pow);
}

void drone::set_diagonals(short diagonal, double per){
    int pow = per * (THROTTLE_MAX - THROTTLE_MIN) + THROTTLE_MIN;
    switch (diagonal)
    {
    case FLBR_DIAGONAL:
        pca9685::set_pwm_ms(MOTOR_FL, pow);
        pca9685::set_pwm_ms(MOTOR_BR, pow);
        
        break;
    case FRBL_DIAGONAL:
        pca9685::set_pwm_ms(MOTOR_FR, pow);
        pca9685::set_pwm_ms(MOTOR_BL, pow);
        break;
    
    default:
        break;
    }
}

void drone::set_motor(short motor, double per){
    int pow = per * (THROTTLE_MAX - THROTTLE_MIN) + THROTTLE_MIN;
    switch(motor){
    case MOTOR_FL:
        pca9685::set_pwm_ms(MOTOR_FL, pow);
        break;
    case MOTOR_FR:
        pca9685::set_pwm_ms(MOTOR_FR, pow);
        break;
    case MOTOR_BL:
        pca9685::set_pwm_ms(MOTOR_BL, pow);
        break;
    case MOTOR_BR:
        pca9685::set_pwm_ms(MOTOR_BR, pow);
        break;
    default:break;
    }
}

void drone::destroy(){
    
    set_all(0);

    pca9685::destroy();
}