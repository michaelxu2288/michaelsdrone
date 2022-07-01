#include <drone.h>

void exit(){
    drone::destroy_sensors();
}

int main(){
    std::atexit(exit);

    drone::init_sensors();
    while(1);

}