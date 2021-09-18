#include <gamepad.h>
#include <iostream>
#include <unistd.h>

int main(){
    gamepad::init();
    char a = a;
    std::cout << "Enter a character to exit: ";
    std::cin >> a;
    gamepad::stop();
}