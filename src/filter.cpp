#include <filter.h>
#include <cmath>


double filter::low_pass::operator[] (double x0){
    double y0 = b0 * x0 + b1 * x1 + b2 * x2 + a1 * y1 + a2 * y2;
    x2 = x1;
    x1 = x0;
    y2 = y1;
    y1 = y0;
    return y0;
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

    x1=x2=y1=y2=0;
}


filter::low_pass::low_pass() {
    a1=a2=b0=b1=b2 = 0;
    x1=x2=y1=y2=0;
}
