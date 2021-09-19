#include <iostream>
#include <unistd.h>
#include <thread>
#include <ctime>
#include <gamepad.h>
#include <drone.h>

double fall_back_throttle = 0;
double throttle = 0;
double dt = 0.01;

int main(){
    setvbuf(stdout, NULL,_IONBF,0);

    drone::init();
    gamepad::init();

    bool update = false;

    while(true){

        if(gamepad::get_button(0)){
            throttle += 1 * dt;
            update = true;
        }
        if(gamepad::get_button(1)){
            throttle -= 1 * dt;
            update = true;
        }

        if(throttle < 0){throttle = 0;}
        if(throttle > 0.3) {throttle = 0.3;}
        
        if(update){
            drone::set_all(throttle);
            update = false;
        }

        usleep(10000);
    };

    drone::destroy();
}