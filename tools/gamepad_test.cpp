#include <gamepad.h>
#include <iostream>
#include <unistd.h>

int main(){
    gamepad::init();

    char n;
    std::cin >> n;

    gamepad::stop();
}