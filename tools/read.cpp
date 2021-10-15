#include <mpu6050.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <socket.h>
#include <thread>
#include <cstdio>
#include <cstdlib>

math::vector euler_glob;

void exit(){
    if(remove("/run/test") < 0){
        perror("Couldn't cleanup /run/test");
    }
}

int t_since;
void thread() {
    
    std::atexit(exit);

    sock::socket client(sock::unix, sock::tcp);

    sock::un_connection unix_connection = client.un_connect("/run/test");
    char recv[1024];

    while(1){
        sprintf(recv, "%d %f %f %f", t_since, euler_glob.x, euler_glob.y, euler_glob.z);
        printf("%d %9.7f %9.7f %9.7f %7.4f %7.4f %7.4f\n", t_since, euler_glob.x*RAD_TO_DEG, euler_glob.y*RAD_TO_DEG, euler_glob.z*RAD_TO_DEG);

        unix_connection.send(recv,strlen(recv));
        usleep(10000);
    }
}

int main(){

    std::thread socket_thread(thread);

    mpu6050::init();
    mpu6050::set_accl_set(mpu6050::accl_range::g_2);
    mpu6050::set_gyro_set(mpu6050::gyro_range::deg_2000);
    mpu6050::set_clk(mpu6050::clk::y_gyro);
    mpu6050::set_fsync(mpu6050::fsync::input_dis);
    mpu6050::set_dlpf_bandwidth(mpu6050::dlpf::hz_5);
    mpu6050::wake_up();

    mpu6050::set_offsets(-77, 240, -32334, 27, -29, -16);
    //mpu6050::set_offsets(1510, -170, -1350, -101, 69, 40);
    //mpu6050::set_offsets(1392, 18, -1359, -100, 68, 38);
    //mpu6050::set_offsets(1441, 87, -1340, -197, 149, 78)
    //mpu6050::set_offsets(1360, -158, -1345, -199, 138, 72);
    //mpu6050::set_offsets(1377, 161, -1343, -200, 138, 68);

    math::quarternion rotation(1,0,0,0), euler_q;
    math::vector euler_v;
    double data[6];
    int i = 0;
    auto then = std::chrono::steady_clock::now();
    auto start = then;
    auto now = std::chrono::steady_clock::now();
    while(1) {
        usleep(10);
        mpu6050::read(data);
        now = std::chrono::steady_clock::now();
        double dt = std::chrono::duration_cast<std::chrono::milliseconds> (now - then).count() * 0.001;
        t_since = std::chrono::duration_cast<std::chrono::milliseconds> (now - start).count();
        then = now;
        euler_v = math::vector(data[3]*dt*DEG_TO_RAD, data[4]*dt*DEG_TO_RAD, data[5]*dt*DEG_TO_RAD);
        euler_q = math::quarternion::fromEulerZYX(euler_v);
        rotation = euler_q*rotation;

        euler_glob = math::quarternion::toEuler(rotation);		
    }
}