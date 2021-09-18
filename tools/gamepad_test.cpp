#include <gamepad.h>
#include <iostream>
#include <unistd.h>

int main(){
    gamepad::init();

    while(true){
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        std::cout << "Axis 0: " << gamepad::get_axis(0) << "\n";

        usleep(100000);
    }

    gamepad::stop();
}