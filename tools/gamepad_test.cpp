#include <gamepad.h>
#include <iostream>
#include <unistd.h>

int main(){
    gamepad::init();

    while(true){
        printf("Axis 0: %5.3f | Axis 1: %5.3f | Axis 2: %5.3f | Axis 3: %5.3f | Axis 4: %5.3f | Axis 5: %5.3f\n", gamepad::get_axis(0), gamepad::get_axis(1), gamepad::get_axis(2), gamepad::get_axis(3), gamepad::get_axis(4), gamepad::get_axis(5));
        usleep(100000);
    }
    gamepad::stop();
}