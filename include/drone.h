#ifndef DRONE_H
#define DRONE_H
#include <settings.h>
#include <unistd.h>

namespace drone {

    extern bool running;


    /**
     * DRONE SETTINGS
     * */
    void init();
    void enable(bool fl, bool fr, bool bl, bool br);
    void destroy();

    void force_terminate();

    /**
     * MOTOR SETTINGS
     * */
    void set_all(double width);
    void set_diagonals(short diagonal, double width);
    void set_motor(short motor, double width);

    
    /**
     * PRE PROGRAMMED ROUTINES
     * */
    void arm();




};




#endif