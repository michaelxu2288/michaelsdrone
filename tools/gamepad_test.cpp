#include <gamepad.h>
#include <iostream>
#include <unistd.h>

int main(){
    gamepad::init();

    while(true){
        printf("Axis 0: %f\n", gamepad::get_axis(0));
        usleep(100000);
    }
    gamepad::stop();
}