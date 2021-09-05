#include <pca9685.h>
#include <unistd.h>
#include <cstdio>

int main(){
    pca9685::init();
    pca9685::wake_up();
    usleep(500);

    printf("Mode 1 reg: %d", pca9685::query_reg(MODE_1));

    pca9685::close();
}