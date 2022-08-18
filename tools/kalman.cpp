#include <kalman.h>
#include <logger.h>
#include <armadillo>
#include <timer.h>
#include <unistd.h>

int ref_rate = 50;
double true_temp = 30.0;
double temp_std_dev = 4.0;
std::default_random_engine generator;
std::normal_distribution<double> dist(true_temp, temp_std_dev);
kalman f(1,1);
timer t;

void loop() {
    double sample = dist();
    // f.predict();
    // f.update(sample);
    logger::info("true: {:10f} | samp: {:10f} | filt: {:10f}", true_temp, sample, f.state(1,1));
}

int main() {

    t.start(loop, 1000 / ref_rate);
    while(t.resume) {
        usleep(10000);
    }
    t.stop();
}