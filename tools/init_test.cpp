#include <drone.h>
#include <unistd.h>

int main(){
    drone::init();

    while(true){
        usleep(1000000);
    }

    drone::destroy();
}