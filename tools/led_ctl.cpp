#include <pca9685.h>
#include <settings.h>
#include <string.h>
#include <stdlib.h>

void toggle(int pin){
    // pca9685::init();
    // pca9685::set_frequency(50);
    // pca9685::wake_up();

}

void set(int pin, double percent){
    pca9685::init();
    pca9685::set_frequency(50);
    pca9685::wake_up();
    pca9685::set_pwm_ms(pin, (int) (percent * (PWM_FULL-1)));
}

int main(int argc, char ** argv){
    if(argc >= 3){
        char * led_type = argv[1];

        if(!strcmp("run", led_type)){
            set(LED_RUN_PIN, atof(argv[2]));
        }else if(!strcmp("error", led_type)){
            set(LED_ERROR_PIN, atof(argv[2]));
        }else if(!strcmp("auto", led_type)){
            set(LED_AUTO_PIN, atof(argv[2]));
        }else if(!strcmp("blue", led_type)){
            set(LED_BLUE_PIN, atof(argv[2]));
        }else if(!strcmp("white", led_type)){
            set(LED_WHITE_PIN, atof(argv[2]));
        }
    }
}