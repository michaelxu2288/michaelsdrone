#include <logger.h>
#include <bmp390.h>
#include <filter.h>
#include <timer.h>
#include <unistd.h>
#include <parameters.h>
#include <socket.h>
#include <cmath>

timer t;
filter::filter low_pass = filter::low_pass(50, 5);
double out = 0;
double raw = 0;
double i = 0;

double data[3];
void loop() {
    i += t.dt;
    // raw = sin(11 * i) / 5 + sin(3 * i);
    
    bmp390::get_data(data);
    // raw = data[1];
    raw = data[2];
    out = low_pass[raw];
    // out = bmp390::get_height(data[0], low_pass[raw]);
}



int main() {
    t.start(loop, 1000 / 50);
    parameters::bind_dbl("raw", &raw, true);
    parameters::bind_dbl("out", &out, true);


    logger::info("Initializing BMP390.");
    bmp390::init();
    bmp390::soft_reset();
    bmp390::set_oversample(bmp390::oversampling::STANDARD, bmp390::ULTRA_LOW_POWER);
    bmp390::set_iir_filter(bmp390::COEFF_3);
    bmp390::set_output_data_rate(bmp390::hz50);
    bmp390::set_enable(true, true);
    
    // bmp390::set_enable_fifo(true, true);
    // bmp390::set_fifo_stop_on_full(false);

    bmp390::set_pwr_mode(bmp390::NORMAL);
    logger:info("Finished initializing the BMP390.");


    sock::socket client(sock::unix, sock::tcp);
    sock::un_connection unix_connection = client.un_connect("/home/pi/drone/run/drone");

    if(!unix_connection.valid){
        // reconnect_node_server(client, unix_connection);
        logger::crit("No socket server");
    }
    double s = 1000000 / 24;
    while(unix_connection.valid){
        logger::info("Data Rate: {:f} hz", 1/t.dt);

        std::string sendStr = parameters::get_json_report();
        int e = unix_connection.send(sendStr.c_str(), sendStr.length());
        // if(e < 0) {
        //     reconnect_node_server(client, unix_connection);
        // }

        usleep(s);
    }
}