#include <bmp390.h>
#include <logger.h>


int main() {
    logger::info("Initializing BMP390.");
    bmp390::init();
    bmp390::soft_reset();
    bmp390::set_oversample(bmp390::oversampling::HIGH, bmp390::ULTRA_LOW_POWER);
    bmp390::set_iir_filter(bmp390::COEFF_127);
    bmp390::set_output_data_rate(bmp390::hz50);
    bmp390::set_enable(true, true);
    bmp390::set_pwr_mode(bmp390::NORMAL);
    logger:info("Finished initializing the BMP390.");

    double data[3];
    while(1){
        // double press = bmp390::get_press();
        // double height = bmp390::get_height();
        // std::cout << height << "\n";
        bmp390::get_data(data);
        logger::info("Temp: {:10.2f}, Pressure: {:10.2f}, Elevation: {:10.2f}", data[0], data[1], data[2]);
        usleep(1000000);
    }
    
}