#include <drone.h>
#include <logger.h>
#include <stdlib.h>

void exit(){
    logger::info("Closing.");
    drone::destroy_sensors();
}

int main(){
    std::atexit(exit);

    logger::set_level(logger::DEBUG);

    drone::init_messsage_thread();
    drone::init_sensors();
    while(1);

}