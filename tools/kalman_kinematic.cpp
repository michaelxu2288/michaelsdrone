#include <kalman.h>
#include <logger.h>
#include <armadillo>
#include <timer.h>
#include <unistd.h>
#include <parameters.h>
#include <socket.h>
#include <string>

int ref_rate = 50;
double true_p = 0.0, true_v = 0.0, true_a = 0.0;
double p_std_dev = 0.1, a_std_dev = 0.1;

std::default_random_engine generator;
std::normal_distribution<double> dist_p(0, p_std_dev), dist_a(0, a_std_dev);

kalman f(2,3);

timer t;
std::string socket_path = "/home/pi/drone/run/drone";
arma::mat measure(2,1);

double out, sample_p, sample_a;

double filt_p, filt_v, filt_a;

sock::un_connection unix_connection;

void loop() {
    // sample = dist(generator);
    // measure(0,0) = sample;
    // f.predict();
    // f.update(measure);



    sample_a = dist_a(generator) + true_a;
    sample_p = dist_p(generator) + true_p;

    filt_p = f.state(0,0);
    filt_v = f.state(1,0);
    filt_a = f.state(2,0);

    logger::info("dt: {:6.4f} true: {:5.2f}, {:5.2f}, {:5.2f} | samp: {:10f}, N/A, {:5.2f} | filt: {:10f}, {:5.2f}, {:5.2f}", t.dt, true_p, true_v, true_a, sample_p, sample_a, filt_p, filt_v, filt_a);
    
    // out = f.state(0,0);
    
    std::string sendStr = parameters::get_json_report();
    int e = unix_connection.send(sendStr.c_str(), sendStr.length());
    if(e < 0) {
        // reconnect_node_server(client, unix_connection);
    }
}

int main() {

    // f.observation_uncertainty = arma::mat(1,1);
    // f.observation_uncertainty(0,0) = temp_std_dev * temp_std_dev;
    // f.process_covar = arma::mat(1,1);
    // f.process_covar(0,0) = 10;
    // logger::info("BURHUFHDAUF");

    f.observation_model_mat = arma::mat(3, 2, arma::fill::zeros);
    f.observation_model_mat(0,0) = 1;
    f.observation_model_mat(2,1) = 1;

    f.observation_uncertainty = arma::mat(3, 3, aram::fill::eye);
    f.observation_uncertainty(0,0) = p_std_dev * p_std_dev;
    f.observation_uncertainty(2,2) = a_std_dev * a_std_dev;

    f.process_covar = arma::mat(3,3, arma::fill::eye);


    // f.state.print();
    // return 4358795438079204583796;

    sock::socket client(sock::unix, sock::tcp);
    unix_connection = client.un_connect(socket_path.c_str());

    {
        // parameters::bind_dbl("out", &out, true);
        // parameters::bind_dbl("samp", &sample, true);
        // parameters::bind_dbl("truth", &true_temp, true);
        parameters::bind_dbl("true_p", &true_p, true);
        parameters::bind_dbl("true_v", &true_v, true);
        parameters::bind_dbl("true_a", &true_a, true);
        parameters::bind_dbl("sample_p", &sample_p, true);
        parameters::bind_dbl("sample_a", &sample_a, true);
        parameters::bind_dbl("filt_p", &filt_p, true);
        parameters::bind_dbl("filt_v", &filt_v, true);
        parameters::bind_dbl("filt_a", &filt_a, true);
    }

    t.start(loop, 1000 / ref_rate);
    while(t.running) {
        usleep(10000);
    }
    t.stop();
}