#ifndef KALMAN_H_GUARD
#define KALMAN_H_GUARD

// #include <math.h>
#include <armadillo>
// namespace kalman {

// }

struct kalman {
    // math::vector x; // x - pos, y - vel, z - acc
    // math::vector x_pred;

    arma::mat state, pred_state;
    arma::mat state_update_mat, state_update_mat_t;
    arma::mat observation_noise, process_noise;
    arma::mat covar_proc_noise, covar_obsrv_noise;
    arma::mat process_covar, pred_process_covar;
    arma::mat observation_model_mat, observation_model_mat_t;
    arma::mat kalman_gain;
    arma::mat observation_uncertainty;
    arma::mat control_update_mat;
    // arma::mat temp1, temp2, temp3;
    arma::mat r;
    arma::mat identity1;

    kalman();
    kalman(int n_measurements, int n_states);

    void predict();
    void predict(arma::mat & control_mat);
    void update(arma::mat & measurements);

    static void kinematic1D_state_update_pva(kalman &k, double dt);
    static void kinematic1D_state_update_pv(kalman &k, double dt);

    static void kinematic1D_control_update_a(kalman &k, double dt);
};


struct fuse_position_acceleration {
    double estimated_a = 0;
    double estimated_v = 0;
    double estimated_p = 0;

    double estimated_p_variance = 1;
    double estimated_v_variance = 1;
    double estimated_a_variance = 1;

    double observation_p_variance = 1;
    double observation_v_variance = 1;
    double observation_a_variance = 1;

    double gain_p = 0.0;
    double gain_v = 0.0;
    double gain_a = 0.0;

    double measured_v_gain = 0.0;
    double measured_v = 0.0;
    double measured_v_variance = 0.0;
    double measured_v_from_a = 0.0;
    double measured_v_from_p = 0.0;
    
    double measured_v_variance_from_a = 1;
    double measured_v_variance_from_p = 1;

    // double old_measured_x;

    fuse_position_acceleration(double observation_p_variance, double observation_a_variance);

    void update(double measured_p, double measure_a, double dt);
};


#endif