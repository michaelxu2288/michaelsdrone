#include <bmp390.h>
#include <iostream>
#include <unistd.h>

int main(){

    //I2C::start();
    bmp390::init();
    std::cout << "pwr_ctl" << bmp390::query_register(REG_PWR_CTRL) << "\n";
    int i = 100;
    while(i--){
        // double press = bmp390::get_press();
        double height = bmp390::get_height();
        std::cout << height << "\n";
        usleep(1000000);
    }
    
    bmp390::stop();
}






