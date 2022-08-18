#ifndef KALMAN_H_GUARD
#define KALMAN_H_GUARD

#include <math.h>

// namespace kalman {

// }

struct kalman {
    // math::vector x; // x - pos, y - vel, z - acc
    // math::vector x_pred;

    math::matrix state, pred_state;
    math::matrix state_update_mat, state_update_mat_t;
    math::matrix covar_proc_noise, covar_obsrv_noise;

    math::matrix temp1, temp2, temp3;

    kalman();

    void predict(double dt);
    // void
};

#endif