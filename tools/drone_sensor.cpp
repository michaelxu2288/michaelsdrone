#include <drone.h>
#include <logger.h>

void exit(){
    logger::info("Closing.");
    drone::destroy_sensors();
}

int main(){
    std::atexit(exit);

    drone::init_sensors();
    while(1);

}