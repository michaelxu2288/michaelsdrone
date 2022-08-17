#include <kalman.h>

void kalman1D::predict(double dt) {
    x_pred.x = x.x + dt * x.y + 0.5 * dt * dt * x.z; // x = x + v * dt + (1/2) * a * dt
    x_pred.y = x.y + x.z * dt; // v = v + a * dt
    x_pred.z = x.z; // a = a
}