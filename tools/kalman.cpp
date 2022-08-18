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

arma::mat measure(1,1);

void loop() {
    double sample = dist(generator);
    measure(0,0) = sample;
    f.predict();
    f.update(measure);
    logger::info("true: {:10f} | samp: {:10f} | filt: {:10f}", true_temp, sample, f.state(0,0));
}

int main() {

    f.observation_uncertainty = arma::mat(1,1);
    f.observation_uncertainty(0,0) = 1000;
    f.process_covar = arma::mat(1,1);
    f.process_covar(0,0) = 10;
    // logger::info("BURHUFHDAUF");

    t.start(loop, 1000 / ref_rate);
    while(t.running) {
        usleep(10000);
    }
    t.stop();
}