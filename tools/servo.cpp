#include <pca9685.h>
#include <unistd.h>
#include <cstdio>

int main(){
    pca9685::init();
    pca9685::wake_up();
    usleep(500);

    printf("Mode 1 reg: %d\n", pca9685::query_reg(MODE_1) & 0xFF);

    pca9685::destroy();
}