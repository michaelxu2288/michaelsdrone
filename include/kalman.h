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
    math::matrix observation_noise, process_noise;
    math::matrix covar_proc_noise, covar_obsrv_noise;
    math::matrix estimated_covar;
    math::matrix observation_model_mat, observation_model_mat_t;
    math::matrix kalman_gain;

    math::matrix temp1, temp2, temp3;

    kalman();
    kalman(int n_measurements, int n_states);

    void predict();
    void update();
};

#endif