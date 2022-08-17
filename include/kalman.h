#ifndef KALMAN_H_GUARD
#define KALMAN_H_GUARD

#include <math.h>

// namespace kalman {

// }

struct kalman1D {
    math::vector x; // x - pos, y - vel, z - acc
    math::vector x_pred;

    void predict(double dt);
    void
};

#endif