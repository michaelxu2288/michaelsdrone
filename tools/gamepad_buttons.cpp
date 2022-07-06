#include <gamepad.h>
#include <iostream>
#include <unistd.h>

int main(){
    gamepad::init();

    while(true){
        for(int i = 0; i < 12; i ++){
            printf("Button %d: %d | ", i, (gamepad::get_button(i) ? 1 : 0));
        }
        printf("\n");
        // printf("Axis 0: %5.3f | Axis 1: %5.3f | Axis 2: %5.3f | Axis 3: %5.3f | Axis 4: %5.3f | Axis 5: %5.3f | Axis 6: %5.3f | Axis 7: %5.3f\n", gamepad::get_axis(0), gamepad::get_axis(1), gamepad::get_axis(2), gamepad::get_axis(3), gamepad::get_axis(4), gamepad::get_axis(5), gamepad::get_axis(6), gamepad::get_axis(7));
        usleep(100000);
    }
    gamepad::stop();
}