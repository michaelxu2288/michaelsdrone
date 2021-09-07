#include <pca9685.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

int main(int argc, char ** argv){

    int mode = 0;
    double percent = 0.5;
    int ms = 0;
    if(argc >= 2){
        mode = atoi(argv[1]);
        if(mode == 0){
            if(argc >= 3){
                ms = atoi(argv[2]);
            }
        }else{
            if(argc >= 3){
                percent = atof(argv[2]);
            }
        }
    }

    pca9685::init();
    pca9685::set_frequency(50);
    printf("PRESCALE register is: %d\n", pca9685::query_reg(PRESCALE) & 0xFF);
    pca9685::wake_up();
    usleep(500);

    printf("Mode 1 reg: %d\n", pca9685::query_reg(MODE_1) & 0xFF);
    // pca9685::set_pwm_off(15, 205);
    // pca9685::set_pwm_on(15, 0);
    if(mode == 0){
        pca9685::set_pwm_ms(15, ms);
    }else{

        pca9685::set_pwm_percent(15, percent);
    }
    
    usleep(500);
    printf("OFF registers are: %3d %3d\n", pca9685::query_reg(PWM15_OFF_H) & 0xFF, pca9685::query_reg(PWM15_OFF_L) & 0xFF);
    printf("ON registers are: %3d %3d\n", pca9685::query_reg(PWM15_ON_H) & 0xFF, pca9685::query_reg(PWM15_ON_L) & 0xFF);

    usleep(5000000);

    pca9685::destroy();
}