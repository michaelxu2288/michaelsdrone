#include <drone.h>
#include <cstdlib>
#include <unistd.h>
#include <settings.h>

int main(int argc, char ** argv){

    if(argc < 2){
        printf("Specify a motor speed as an integer of percent of 100.\n");
        return -1;
    }
    double percent = atof(argv[1]) / 100;
    // int cycle = THROTTLE_MIN + (int)((THROTTLE_MAX-THROTTLE_MIN) * percent);
    // default 1 second
    double timelen = 1;
    if(argc >= 3){
        timelen = (int) (atof(argv[2]));
    }

    printf("Setting all motors to %2f%% power for %d seconds.\n", percent*100, timelen);
    timelen *= 1000000;

    drone::init();

    drone::set_all(percent);

    usleep((int) timelen);

    drone::set_all(0);

    return 0;
}