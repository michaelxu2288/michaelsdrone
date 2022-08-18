#include <kalman.h>

kalman::kalman(int n_measurements, int n_states) {
    state_update_mat = math::matrix::matrix(n_measurements, n_measurements);
    state_update_mat_t = math::matrix::matrix(n_measurements, n_measurements);
}

void kalman1D::predict() {

}