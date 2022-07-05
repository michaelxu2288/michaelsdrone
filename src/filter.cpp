#include <filter.h>
#include <cmath>
#define M_PI 3.14159265359

double filter::low_pass::operator[] (double sensor_raw) const {
    return 0;
}

filter::low_pass::low_pass(double sample_rate, double w0) {
    double ff = w0 / sample_rate;
    const double ita =1.0/ tan(M_PI*ff);
    const double q=sqrt(2.0);
    b0 = 1.0 / (1.0 + q*ita + ita*ita);
    b1= 2*b0;
    b2= b0;
    a1 = 2.0 * (ita*ita - 1.0) * b0;
    a2 = -(1.0 - q*ita + ita*ita) * b0;
}
