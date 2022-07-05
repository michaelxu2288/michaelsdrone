#ifndef FILTER_H_GUARD
#define FILTER_H_GUARD

namespace filter {

    struct low_pass {
        double a1, a2, b0, b1, b2;
        low_pass(double sample_rate, double w0);
        double operator [](double inp) const;
    };
};
#endif