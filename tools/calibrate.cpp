#include <mpu6050.h>
#include <unistd.h>
#include <iostream>
#include <rapidjson/document.h>
#include <fstream>

int main(){
    mpu6050::init();
    mpu6050::print_debug();
    mpu6050::set_accl_set(mpu6050::accl_range::g_2);
    mpu6050::set_gyro_set(mpu6050::gyro_range::deg_2000);
    mpu6050::set_clk(mpu6050::clk::y_gyro);
    mpu6050::set_fsync(mpu6050::fsync::input_dis);
    mpu6050::set_dlpf_bandwidth(mpu6050::dlpf::hz_5);
    mpu6050::wake_up();
    mpu6050::print_debug();
    
    printf("[Output] Calibrating MPU6050\n");
	printf("[Output]  X Accl  |  Y Accl  |  Z Accl  |  X Gyro  |  Y Gyro  |  Z Gyro \n");
    mpu6050::calibrate(2000);
    printf("[Output] Writing to config.json file\n");

    std::ifstream file("/dat/config.json");
    // file.seekg(0, std::ios::end);
    // size_t size = file.tellg();
    // char buf[size];
    // file.seekg(0);
    // file.read(buf, size);
    // rapidjson::Document config;
    // config.Parse(buf);
    // config["sensors]["accel"];



    // usleep(10000000);
    // double data[6];
    // int i = 0;
    // while(1) {
    //     mpu6050::read(data);
    //     if(i++ == 30){
    //         i = 0;
	//         printf("\n[Output]  X Accl  |  Y Accl  |  Z Accl  |  X Gyro  |  Y Gyro  |  Z Gyro \n");
    //     }
    //     printf("[Output] %8.5f | %8.5f | %8.5f | %8.5f | %8.5f | %8.5f\n",data[0],data[1],data[2],data[3],data[4],data[5]);
		
    //     usleep(100000);
    // }
}