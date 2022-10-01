#include <drone.h>
#include <logger.h>
#include <stdlib.h>
#include <thread>
#include <gamepad.h>
#include <unistd.h>
#include <config.h>
#include <csignal>
void bruh(){
    logger::info("Closing.");
    drone::destroy_sensors();
    drone::destroy_message_thread();
    drone::destroy();
    exit(0);
}

void handle(int sig){
    bruh();
}

void ctrl_thrd(){
    config::load_file();
    double deg_t_rad = 3.141592 / 180;
    double roll = config::get_config_dbl("ctrlmax_roll", 5) * deg_t_rad;
    double pitch = config::get_config_dbl("ctrl.max_pitch", 5) * deg_t_rad;
    double vz = config::get_config_dbl("ctrl.max_vz", 5) * deg_t_rad;
    double vyaw = config::get_config_dbl("ctrl.max_vyaw", 5) * deg_t_rad;


    config::write_to_file();

    gamepad::init_wo_connect();

    pid * roll_ctrl = drone::get_roll_controller();
    pid * pitch_ctrl = drone::get_pitch_controller();
    pid * vyaw_ctrl = drone::get_vyaw_controller();
    double * trim = drone::get_trim_ptr();
    double * thrust = drone::get_thrust_ptr();
    double * yawthrust = drone::get_yawthrust_ptr();
    double * front_multiplier = drone::get_front_multiplier_ptr();
    double trimsens = 0.1;
    double maxthrust = 0.1;

    // pid * z_ctrl = drone::get_z_controller();
    while(1){
        roll_ctrl->change_setpoint(-gamepad::get_axis(3) * roll);
        pitch_ctrl->change_setpoint(-gamepad::get_axis(4) * pitch);
        // z_ctrl->change_setpoint(z_ctrl->setpoint + gamepad::get_axis(1) * vz);
        // vyaw_ctrl->change_setpoint(-((0.5 + 0.5 * gamepad::get_axis(2)) - (0.5 + 0.5 * gamepad::get_axis(5))) * vyaw);
        
        (*thrust) = -gamepad::get_axis(1) * maxthrust;
        (*trim) -= trimsens * gamepad::get_axis(7) * 0.01;
        (*front_multiplier) -= trimsens * gamepad::get_axis(6) * 0.01;
        (*yawthrust) = -((0.5 + 0.5 * gamepad::get_axis(2)) - (0.5 + 0.5 * gamepad::get_axis(5))) * vyaw;
        // logger::info("{} {}", *thrust, *trim);

        usleep(10000);
    }
    gamepad::stop();
}

int main(){
    signal(SIGINT, handle);  
    std::atexit(bruh);

    drone::init();

    std::thread ctrl = std::thread(ctrl_thrd);
    // logger::set_level(logger::DEBUG);
    drone::load_configuration();
    drone::init_messsage_thread();
    drone::init_sensors();
    while(1);

}