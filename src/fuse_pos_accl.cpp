#include <kalman.h>


// struct fuse_position_acceleration {
//     double estimated_a = 0;
//     double estimated_v = 0;
//     double estimated_p = 0;

//     double estimated_p_variance = 1;
    // double estimated_v_variance = 1;
//     double estimated_a_variance = 1;

//     double observation_p_variance = 1;
//     double observation_a_variance = 1;



//     void predict(double dt);
//     void update(double measured_p, double measure_a);
// }


fuse_position_acceleration::fuse_position_acceleration(double _observation_p_variance, double _observation_a_variance) {
    observation_a_variance = _observation_a_variance;
    observation_p_variance = _observation_p_variance;
}

void fuse_position_acceleration::update(double measured_p, double measured_a, double dt) {

    measured_v_from_a = estimated_v + measured_a * dt;
    measured_v_from_p = (measured_p - estimated_p) * dt;

    measured_v_variance_from_p = estimated_p_variance / dt;
    measured_v_variance_from_a = estimated_v_variance + estimated_a_variance * dt;

    measured_v_gain = measured_v_variance_from_p / (measured_v_variance_from_a + measured_v_variance_from_p);

    measured_v = (1 - measured_v_gain) * measured_v_from_p + measured_v_gain * measured_v_from_a;
    measured_v_variance = (1 - measured_v_gain) * measured_v_variance_from_p + measured_v_gain * measured_v_variance_from_a;

    estimated_p += estimated_v * dt + 0.5 * estimated_a * dt * dt;
    estimated_v += estimated_a * dt;

    estimated_p_variance = estimated_p_variance + estimated_v_variance * dt * dt + estimated_a_variance * dt * dt * dt * dt / 4;
    estimated_v_variance = estimated_v_variance + estimated_a_variance * dt * dt;

    gain_p = estimated_p_variance / (estimated_p_variance + observation_p_variance + off);
    gain_v = estimated_v_variance / (estimated_v_variance + measured_v_variance + off);
    gain_a = estimated_a_variance / (estimated_a_variance + observation_a_variance + )off;

    estimated_p += gain_p * (measured_p - estimated_p);
    estimated_v += gain_v * (measured_v - estimated_v);
    estimated_a += gain_a * (measured_a - estimated_a);

    estimated_p_variance = (1 - gain_p) * estimated_p_variance; // + gain_p * observation_p_variance;
    estimated_v_variance = (1 - gain_v) * estimated_v_variance; // + gain_v * measured_v_variance;
    estimated_a_variance = (1 - gain_a) * estimated_a_variance; // + gain_a * observation_a_variance;
}