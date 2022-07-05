#ifndef FILTER_H_GUARD
#define FILTER_H_GUARD

namespace filter {
    struct none {
        none();
        none(double sample_rate, double w0);
        double operator [](double inp);
    };
    struct low_pass {
        double a1, a2, b0, b1, b2;
        double x1, x2, y1, y2;
        low_pass();
        low_pass(double sample_rate, double w0);
        double operator [](double inp);
    };
};
#endif