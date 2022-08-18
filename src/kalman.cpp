#include <kalman.h>
#include <armadillo>

kalman::kalman(int n_measurements, int n_states) {
    state  = arma::mat(n_states, n_states);

    state_update_mat = arma::mat(n_states, n_states, arma::fill::eye);
    state_update_mat_t = arma::mat(n_states, n_states, arma::fill::eye);

    observation_model_mat = arma::mat(n_states, n_measurements, arma::fill::eye);
    observation_model_mat_t = arma::mat(n_measurements, n_states, arma::fill::eye);

    // kalman_gain = arma::mat(n_states, n_states);

    // estimated_covar = arma::mat(n_states, n_states);
    identity1 = arma::mat(n_states, n_states, arma::fill::eye);
}

void kalman::predict() {
    pred_state = state_update_mat * state; // NO NOISE
    // pred_state.print("predicted state");
    pred_process_covar = state_update_mat * process_covar * state_update_mat_t; // NO NOISE
    // pred_process_covar.print("predicted process covariance");
}

void kalman::update(arma::mat measurements) {
    arma::mat temp = pred_process_covar;
    kalman_gain = temp * (temp + observation_uncertainty).i();

    arma::mat y = observation_model_mat * measurements;
    state = pred_state + kalman_gain * (y - pred_state); 

    
    process_covar = (identity1 - kalman_gain) - pred_process_covar;
}