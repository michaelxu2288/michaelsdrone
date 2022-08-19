#include <kalman.h>
#include <logger.h>
#include <armadillo>
#include <timer.h>
#include <unistd.h>
#include <parameters.h>
#include <socket.h>
#include <string>
#include <cmath>

int ref_rate = 50;
double true_temp = 30.0;
double temp_std_dev = 0.001;
std::default_random_engine generator;
std::normal_distribution<double> dist(0.0, temp_std_dev);
kalman f(1,1);
timer t;
std::string socket_path = "/home/pi/drone/run/drone";
arma::mat measure(1,1);

double out, sample, gain, covar;

sock::un_connection unix_connection;
double i = 0;
void loop() {
    i += t.dt;
    true_temp = 10 * sin(i / 2) + 30;
    sample = dist(generator) + true_temp;
    measure(0,0) = sample;
    f.predict();
    f.update(measure);
    
    out = f.state(0,0);
    gain = f.kalman_gain(0,0);
    covar = f.process_covar(0,0);

    logger::info("true: {:5.2f} | samp: {:5.2f} | filt: {:5.2f} | gain: {:5.4f} | covar: {:5f}", true_temp, sample, out, gain, covar);
    
    
    std::string sendStr = parameters::get_json_report();
    int e = unix_connection.send(sendStr.c_str(), sendStr.length());
    if(e < 0) {
        // reconnect_node_server(client, unix_connection);  
    }
}

int main() {

    f.observation_uncertainty = arma::mat(1,1);
    f.observation_uncertainty(0,0) = 0.000000001;// temp_std_dev * temp_std_dev;
    f.process_covar = arma::mat(1,1);
    f.process_covar(0,0) = 10;
    f.r = arma::mat(1,1);
    f.r(0,0) = 0;
    // logger::info("BURHUFHDAUF");

    sock::socket client(sock::unix, sock::tcp);
    unix_connection = client.un_connect(socket_path.c_str());

    {
        parameters::bind_dbl("out", &out, true);
        parameters::bind_dbl("samp", &sample, true);
        parameters::bind_dbl("truth", &true_temp, true);
        parameters::bind_dbl("gain", &gain, true);
        parameters::bind_dbl("covar", &covar, true);
        // parameters::bind_dbl("", &, true);
    }

    t.start(loop, 1000 / ref_rate);
    while(t.running) {
        usleep(10000);
    }
    t.stop();
}