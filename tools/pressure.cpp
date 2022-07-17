#include <bmp390.h>
#include <iostream>
#include <unistd.h>

#include <logger.h>

int main(){

    //I2C::start();
    bmp390::init();
    std::cout << "pwr_ctl: " << bmp390::query_register(REG_PWR_CTRL) << "\n";
    int i = 100;
    double data[3];
    while(i--){
        // double press = bmp390::get_press();
        // double height = bmp390::get_height();
        // std::cout << height << "\n";
        bmp390::get_data(data);
        logger::info("Temp: {:10.2f}, Pressure: {:10.2f}, Elevation: {:10.2f}", data[0], data[1], data[2]);
        usleep(1000000);
    }
    
    bmp390::stop();
}






