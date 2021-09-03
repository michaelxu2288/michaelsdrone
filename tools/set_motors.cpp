#include <drone.h>
#include <cstdlib>
#include <unistd.h>
#include <settings.h>

int main(int argc, char ** argv){

    if(argc < 3){
        printf("Specify a motor speed as a percent of 100.\n");
        return -1;
    }
    double percent = atof(argv[2]) / 100;
    int cycle = THROTTLE_MIN + (int)((THROTTLE_MAX-THROTTLE_MIN) * percent);
    int timelen = 10;
    if(argc >= 4){
        timelen = atoi(argv[3]);
    }
    timelen *= 1000000;

    drone d;
    d.start();

    d.set_all(cycle);

    usleep(timelen);

    d.close();

    return 0;
}