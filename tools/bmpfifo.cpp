#include <bmp390.h>
#include <logger.h>
#include <unistd.h>


int main() {
    logger::info("Initializing BMP390.");
    bmp390::init();
    bmp390::soft_reset();
    bmp390::set_oversample(bmp390::oversampling::HIGH, bmp390::ULTRA_LOW_POWER);
    bmp390::set_iir_filter(bmp390::COEFF_3);
    bmp390::set_output_data_rate(bmp390::hz50);
    bmp390::set_enable(true, true);
    bmp390::set_pwr_mode(bmp390::NORMAL);
    logger:info("Finished initializing the BMP390.");

    int burh = 1000000 / 50;
    double data[3];
    // for(int i = 0; i< 10; i ++) {
        
    //     bmp390::get_data(data);
    //     usleep(burh);
    // }
    // double pressure = bmp390::get_press(bmp390::get_temp());
    // logger::info("Pressure benchmark {:10.0f}", pressure);
    // bmp390::set_pressure_benchmark(pressure);
    while(1){
        // double press = bmp390::get_press();
        // double height = bmp390::get_height();
        // std::cout << height << "\n";
        bmp390::get_data(data);
        logger::info("Temp: {:10.2f} C, Pressure: {:10.0f} P, Elevation: {:10.1f} ft", data[0], data[1], data[2] * 3.28084);
        usleep(burh);
    }
    
}