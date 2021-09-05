#include <pca9685.h>
#include <unistd.h>
#include <cstdlib>

int main(){
    pca9685::open();
    pca9685::wake_up();
    usleep(500);
    
    printf("Mode 1 reg: %d", pca9685::query_reg(MODE_1));

    pca9685::close();
}