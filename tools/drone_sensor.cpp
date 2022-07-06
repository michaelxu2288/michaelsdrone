#include <drone.h>
#include <logger.h>
#include <stdlib.h>
#include <thread>
#include <gamepad.h>
#include <unistd.h>
#include <config.h>

void exit(){
    logger::info("Closing.");
    drone::destroy_sensors();
}

void ctrl_thrd(){
    config::load_file();
    double deg_t_rad = 3.141592 / 180;
    double roll = config::get_config_dbl("ctrl_max_roll", 5) * deg_t_rad;
    double pitch = config::get_config_dbl("ctrl_max_pitch", 5) * deg_t_rad;
    config::write_to_file();

    gamepad::init();
    pid * roll_ctrl = drone::get_roll_controller();
    pid * pitch_ctrl = drone::get_pitch_controller();
    while(1){
        roll_ctrl->change_setpoint(gamepad::get_axis(3) * roll);
        pitch_ctrl->change_setpoint(gamepad::get_axis(4) * pitch);
        usleep(10000);
    }
    gamepad::stop();
}

int main(){
    std::atexit(exit);

    std::thread ctrl = std::thread(ctrl_thrd);
    // logger::set_level(logger::DEBUG);
    drone::load_configuration();
    drone::init_messsage_thread();
    drone::init_sensors();
    while(1);

}