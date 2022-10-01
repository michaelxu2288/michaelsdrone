#ifndef GAMEPAD_H
#define GAMEPAD_H

namespace gamepad {

    bool init();
    void init_wo_connect();
    void stop();

    void set_axis(int axis, double value);
    // bool set_button(int button, )

    double get_axis(int axis);
    bool get_button(int button);

    double * get_axis_ptr(int axis);
}

#endif