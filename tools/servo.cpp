#include <pca9685.h>
#include <unistd.h>
#include <cstdio>

int main(){
    pca9685::init();
    pca9685::wake_up();
    usleep(500);

    printf("Mode 1 reg: %d\n", pca9685::query_reg(MODE_1) & 0xFF);
    pca9685::set_frequency(50);
    printf("PRESCALE register is: %d\n", pca9685::query_reg(MODE_1) & 0xFF);
    // pca9685::set_pwm_off(15, 205);
    // pca9685::set_pwm_on(15, 0);
    pca9685::set_pwm_ms(15, 1);

    usleep(5000000);

    pca9685::destroy();
}