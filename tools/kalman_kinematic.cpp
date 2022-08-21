#include <kalman.h>
#include <logger.h>
#include <armadillo>
#include <timer.h>
#include <unistd.h>
#include <parameters.h>
#include <socket.h>
#include <string>
#include <mpu6050.h>
#include <bmp390.h>

#include <cmath>

#include <settings.h>

int ref_rate = 50;
double true_p = 0.0, true_v = 0.0, true_a = 0.0;
double p_std_dev = sqrt(ALTITUDE_VARIANCE), a_std_dev = sqrt(ACCEL_VARIANCE);

std::default_random_engine generator;
std::normal_distribution<double> dist_p(0, p_std_dev), dist_a(0, a_std_dev);

kalman f(2,2);

fuse_position_acceleration pa(p_std_dev * p_std_dev, a_std_dev * a_std_dev);

timer t;
std::string socket_path = "/home/pi/drone/run/drone";
arma::mat measure(1,1);
arma::mat control_mat(1,1);

double out, sample_p, sample_a;

double filt_p, filt_v, filt_a;

double mpu[6];
double bmp[3];

sock::un_connection unix_connection;
double i = 0;
void loop() {
    // sample = dist(generator);
    // measure(0,0) = sample;
    // f.predict();
    // f.update(measure);
    i += t.dt;

    // true_p = 10 * sin(i);
    // true_v = 10 * cos(i);
    // true_a = -10 * sin(i);

    mpu6050::read(mpu);
    bmp390::get_data(bmp);

    sample_a = -(mpu[2] - 1) * 9.81;
    sample_p = bmp[2];

    // sample_a = dist_a(generator) + true_a;
    // sample_p = dist_p(generator) + true_p;

    // measure(0,0) = sample_p;
    // measure(1,0) = sample_a;

    // control_mat(0,0) = sample_a;

    // kalman::kinematic1D_state_update_pv(f, t.dt);
    // kalman::kinematic1D_control_update_a(f, t.dt);

    // f.predict(control_mat);
    // f.update(measure);

    // filt_p = f.state(0,0);
    // filt_v = f.state(1,0);
    // filt_a = f.state(2,0);

    pa.update(sample_p, sample_a, t.dt);

    filt_p = pa.estimated_p;
    filt_v = pa.estimated_v;
    filt_a = pa.estimated_a;

    logger::info("dt: {:6.4f} true: {:5.2f}, {:5.2f}, {:5.2f} | samp: {:10f}, N/A, {:5.2f} | filt: {:10f}, {:5.2f}, {:5.2f} | var: {:5.2f} {:5.2f} {:5.2f}", t.dt, true_p, true_v, true_a, sample_p, sample_a, filt_p, filt_v, filt_a, pa.estimated_p_variance, pa.estimated_v_variance, pa.estimated_a_variance);
    
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

    // f.observation_model_mat = arma::mat(2, 1, arma::fill::zeros);
    // f.observation_model_mat(0,0) = 1;
    // // f.observation_model_mat(2,1) = 1;

    // f.observation_uncertainty = arma::mat(2, 2, arma::fill::zeros);
    // f.observation_uncertainty(0,0) = p_std_dev * p_std_dev;
    // // f.observation_uncertainty(2,2) = a_std_dev * a_std_dev;

    // f.process_covar = arma::mat(2,2, arma::fill::eye);

    // f.r = arma::mat(2,2, arma::fill::zeros);
    // f.r(0,0) = 0.01;
    // f.r(1,1) = 0.01;

    // f.state.print();
    // return 4358795438079204583796;

    {
        {
            logger::info("Initializing MPU6050.");
            mpu6050::init();
            mpu6050::set_accl_set(mpu6050::accl_range::g_2);
            mpu6050::set_gyro_set(mpu6050::gyro_range::deg_250);
            mpu6050::set_clk(mpu6050::clk::y_gyro);
            mpu6050::set_fsync(mpu6050::fsync::input_dis);
            mpu6050::set_dlpf_bandwidth(mpu6050::dlpf::hz_5);
            mpu6050::wake_up();
            mpu6050::calibrate(7);
            logger::info("Finished intializing the MPU6050.");
        }
        {
            logger::info("Initializing BMP390.");
            bmp390::init();
            bmp390::soft_reset();
            bmp390::set_oversample(bmp390::oversampling::STANDARD, bmp390::ULTRA_LOW_POWER);
            bmp390::set_iir_filter(bmp390::COEFF_3);
            bmp390::set_output_data_rate(bmp390::hz50);
            bmp390::set_enable(true, true);
            
            bmp390::set_enable_fifo(true, true);
            bmp390::set_fifo_stop_on_full(false);

            bmp390::set_pwr_mode(bmp390::NORMAL);
            logger:info("Finished initializing the BMP390.");
        }
    }

    sock::socket client(sock::unix, sock::tcp);
    unix_connection = client.un_connect(socket_path.c_str());

    {
        // parameters::bind_dbl("out", &out, true);
        // parameters::bind_dbl("samp", &sample, true);
        // parameters::bind_dbl("truth", &true_temp, true);
        // parameters::bind_dbl("true_p", &true_p, true);
        // parameters::bind_dbl("true_v", &true_v, true);
        // parameters::bind_dbl("true_a", &true_a, true);
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