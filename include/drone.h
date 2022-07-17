#ifndef DRONE_H
#define DRONE_H
#include <settings.h>
#include <string>
#include <math.h>
#include <pid.h>
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

    /**
     * @brief Set all the motors to a certain percent power.
     * 
     * @param percent
     */
    void set_all(double percent);
    void set_diagonals(short diagonal, double percent);
    void set_motor(short motor, double percent);

    
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

    void init_messsage_thread(bool threaded = true);
    void destroy_message_thread();

    void init_sensors(bool threaded = true);
    void destroy_sensors();

    void get_orientation(math::quarternion &quart);
    void get_orientation(math::vector &euler);
    void get_position(math::vector &pos);

    void load_configuration();

    void synch_loop();

    void set_ctrller_connected_flag(bool connected);

    pid * get_roll_controller();
    pid * get_pitch_controller();
    pid * get_z_controller();
    pid * get_vyaw_controller();

    double get_trim();
    void set_trim(double val);
    double * get_trim_ptr();

    double * get_thrust_ptr();
    double * get_yawthrust_ptr();
};




#endif