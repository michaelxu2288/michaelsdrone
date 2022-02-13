#ifndef DRONE_H
#define DRONE_H
#include <settings.h>
#include <string>

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

    void set_trim(short motor, double trim);
    void set_diagonal_trim(short diagonal, double trim);
    void set_all_trim(double trim);


    void run_command(const std::string& s);
    void run_command(const std::string& s, std::string& msg);

    void set_throttle(double power);
};




#endif