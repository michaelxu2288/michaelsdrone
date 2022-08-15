#include <logger.h>
#include <bmp390.h>
#include <filter.h>
#include <timer.h>
#include <unistd.h>
#include <parameters.h>
#include <socket.h>

timer t;
filter::filter low_pass = filter::low_pass(24, 5);
double out = 0;


void loop() {
    out += t.dt;
}



int main() {
    t.start(loop, 1000 / 24);
    parameters::bind_dbl("out", &out, true);

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