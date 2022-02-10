#ifndef PID_H
#define PID_H

#define I_MAX_ERR 10.0
#define I_MAX 10.0

struct pid {

    double p = 0.0;
    double i = 0.0;
    double d = 0.0;

    double i_curr = 0.0;
    double setpoint = 0.0;
    double old_error = 0.0;

    double i_max = I_MAX;
    double i_max_err = I_MAX_ERR;

    pid();
    pid(double p_p, double p_i, double p_d);


    double output(double curr, double dt);
    void change_setpoint(double new_setpoint);

    void reset_integral_term();



};

#endif















