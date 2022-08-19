#include <kalman.h>
#include <logger.h>
#include <armadillo>
#include <timer.h>
#include <unistd.h>
#include <parameters.h>
#include <socket.h>
#include <string>

int ref_rate = 50;
double true_temp = 30.0;
double temp_std_dev = 0.0001;
std::default_random_engine generator;
std::normal_distribution<double> dist(true_temp, temp_std_dev);
kalman f(1,1);
timer t;
std::string socket_path = "/home/pi/drone/run/drone";
arma::mat measure(1,1);

double out, sample;

sock::un_connection unix_connection;

void loop() {
    sample = dist(generator);
    measure(0,0) = sample;
    f.predict();
    f.update(measure);
    logger::info("true: {:10f} | samp: {:10f} | filt: {:10f}", true_temp, sample, f.state(0,0));
    
    out = f.state(0,0);
    
    std::string sendStr = parameters::get_json_report();
    int e = unix_connection.send(sendStr.c_str(), sendStr.length());
    if(e < 0) {
        // reconnect_node_server(client, unix_connection);
    }
}

int main() {

    f.observation_uncertainty = arma::mat(1,1);
    f.observation_uncertainty(0,0) = 0.0003;
    f.process_covar = arma::mat(1,1);
    f.process_covar(0,0) = 10;
    // logger::info("BURHUFHDAUF");

    sock::socket client(sock::unix, sock::tcp);
    unix_connection = client.un_connect(socket_path.c_str());

    {
        parameters::bind_dbl("out", &out, true);
        parameters::bind_dbl("samp", &sample, true);
        parameters::bind_dbl("truth", &true_temp, true);
    }

    t.start(loop, 1000 / ref_rate);
    while(t.running) {
        usleep(10000);
    }
    t.stop();
}