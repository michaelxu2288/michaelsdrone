#include <kalman.h>

kalman::kalman(int n_measurements, int n_states) {
    state_update_mat.setup(n_measurements, n_measurements);
    state_update_mat_t.setup(n_measurements, n_measurements);
}

void kalman::predict() {

}