#include <drone.h>
#include <logger.h>
#include <stdlib.h>
#include <thread>
#include <gamepad.h>
#include <unistd.h>
void exit(){
    logger::info("Closing.");
    drone::destroy_sensors();
}

void ctrl_thrd(){
    gamepad::init();
    pid * roll_ctrl = drone::get_roll_controller();
    pid * pitch_ctrl = drone::get_pitch_controller();
    double deg_5 = 5 * 3.141592 / 180;
    while(1){
        roll_ctrl->change_setpoint(gamepad::get_axis(3) * deg_5);
        pitch_ctrl->change_setpoint(gamepad::get_axis(4) * deg_5);
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