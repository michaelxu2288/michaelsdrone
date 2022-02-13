#include <drone.h>
#include <drone_server.h>

#include <unistd.h>

int main(){

    drone::init_server();

    while(true){
        usleep(10000);
    }

}