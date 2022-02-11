#include <pca9685.h>
#include <settings.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

int main(int argc, char ** argv){
    if(argc == 3){
        char * led_type = argv[1];
        double power = atof(argv[2]);

        pca9685::init();
        pca9685::set_frequency(50);
        pca9685::wake_up();

        if(!strcmp("fl", led_type)){
            // set(LED_RUN_PIN, atof(argv[2]));
            pca9685::set_pwm_ms(MOTOR_FL_PIN, (THROTTLE_MAX - THROTTLE_MIN) * power + THROTTLE_MIN);
        }else if(!strcmp("fr", led_type)){
            // set(LED_ERROR_PIN, atof(argv[2]));
            pca9685::set_pwm_ms(MOTOR_FR_PIN, (THROTTLE_MAX - THROTTLE_MIN) * power + THROTTLE_MIN);
        }else if(!strcmp("bl", led_type)){
            // set(LED_AUTO_PIN, atof(argv[2]));
            pca9685::set_pwm_ms(MOTOR_BL_PIN, (THROTTLE_MAX - THROTTLE_MIN) * power + THROTTLE_MIN);
        }else if(!strcmp("br", led_type)){
            // set(LED_BLUE_PIN, atof(argv[2]));
            pca9685::set_pwm_ms(MOTOR_BR_PIN, (THROTTLE_MAX - THROTTLE_MIN) * power + THROTTLE_MIN);
        }
    }else {
        std::cout << "motor [fl|fr|bl|br] <power>\n";
    }
}